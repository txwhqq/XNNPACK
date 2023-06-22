// Copyright 2023 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <algorithm>  // For std::generate, std::min.
#include <array>      // For std::array.
#include <cmath>      // For std::lrintf.
#include <cstddef>    // For size_t.
#include <cstdint>    // For uint32_t.
#include <limits>     // For std::numeric_limits.
#include <memory>     // For std::unique_ptr.
#include <numeric>    // For std::accumulate.
#include <random>     // For std::random_device, std::mt19937, std::uniform_real_distribution.
#include <vector>     // For std::vector.

#include <xnnpack.h>
#include <xnnpack/operator.h>
#include <xnnpack/requantization.h>
#include <xnnpack/subgraph.h>

#include <gtest/gtest.h>

template <class T> class RoPETestBase : public ::testing::Test {
protected:
  RoPETestBase()
  {
    random_device = std::make_unique<std::random_device>();
    rng = std::mt19937((*random_device)());
    f32dist = std::uniform_real_distribution<float>(0.1f, 1.0f);
    dim_dist = std::uniform_int_distribution<size_t>(5, 15);

    batch_size = dim_dist(rng);
    sequence_size = dim_dist(rng);
    do {
      max_sequence_size = dim_dist(rng);
    } while (max_sequence_size < sequence_size);
    heads = dim_dist(rng);
    channels = dim_dist(rng) * 2;  // ensure the number of channels is even

    input = std::vector<T>(XNN_EXTRA_BYTES / sizeof(T) + batch_size * sequence_size * heads * channels);
    weights = std::vector<T>(max_sequence_size * channels);
    operator_output = std::vector<T>(batch_size * sequence_size * heads * channels);
    subgraph_output = std::vector<T>(operator_output.size());
  }

  std::unique_ptr<std::random_device> random_device;
  std::mt19937 rng;
  std::uniform_real_distribution<float> f32dist;
  std::uniform_int_distribution<size_t> dim_dist;

  size_t batch_size;
  size_t max_sequence_size;
  size_t sequence_size;
  size_t heads;
  size_t channels;

  std::vector<T> input;
  std::vector<T> weights;
  std::vector<T> operator_output;
  std::vector<T> subgraph_output;
};

using RoPETestF32 = RoPETestBase<float>;

TEST_F(RoPETestF32, define)
{
  ASSERT_EQ(xnn_status_success, xnn_initialize(/*allocator=*/nullptr));

  xnn_subgraph_t subgraph = nullptr;
  ASSERT_EQ(xnn_status_success, xnn_create_subgraph(3, /*flags=*/0, &subgraph));
  std::unique_ptr<xnn_subgraph, decltype(&xnn_delete_subgraph)> auto_subgraph(subgraph, xnn_delete_subgraph);

  uint32_t input_id = XNN_INVALID_NODE_ID;
  const std::array<size_t, 4> input_dims{{batch_size, sequence_size, heads, channels}};
  ASSERT_EQ(xnn_status_success,
    xnn_define_tensor_value(subgraph, xnn_datatype_fp32, input_dims.size(), input_dims.data(),
                            /*data=*/nullptr, /*external_id=*/0, /*flags=*/XNN_VALUE_FLAG_EXTERNAL_INPUT, &input_id));
  ASSERT_NE(input_id, XNN_INVALID_NODE_ID);

  uint32_t weights_id = XNN_INVALID_NODE_ID;
  const std::array<size_t, 2> weights_dims{{max_sequence_size, channels}};
  ASSERT_EQ(xnn_status_success,
    xnn_define_tensor_value(subgraph, xnn_datatype_fp32, weights_dims.size(), weights_dims.data(),
                            weights.data(), /*external_id=*/1, /*flags=*/0, &weights_id));

  uint32_t output_id = XNN_INVALID_NODE_ID;
  const std::array<size_t, 4> output_dims{{batch_size, sequence_size, heads, channels}};
  ASSERT_EQ(
    xnn_status_success, xnn_define_tensor_value(
                          subgraph, xnn_datatype_fp32, output_dims.size(), output_dims.data(), nullptr,
                          /*external_id=*/2, /*flags=*/XNN_VALUE_FLAG_EXTERNAL_OUTPUT, &output_id));
  ASSERT_NE(output_id, XNN_INVALID_NODE_ID);

  ASSERT_EQ(xnn_status_success,
    xnn_define_rope(subgraph, max_sequence_size, input_id, weights_id, output_id, /*flags=*/0));

  ASSERT_EQ(subgraph->num_nodes, 1);
  const struct xnn_node* node = &subgraph->nodes[0];
  ASSERT_EQ(node->type, xnn_node_type_rope);
  ASSERT_EQ(node->compute_type, xnn_compute_type_fp32);
  ASSERT_EQ(node->params.rope.max_sequence_size, max_sequence_size);
  ASSERT_EQ(node->num_inputs, 2);
  ASSERT_EQ(node->inputs[0], input_id);
  ASSERT_EQ(node->inputs[1], weights_id);
  ASSERT_EQ(node->num_outputs, 1);
  ASSERT_EQ(node->outputs[0], output_id);
  ASSERT_EQ(node->flags, 0);
}

TEST_F(RoPETestF32, matches_operator_api)
{
  ASSERT_EQ(xnn_status_success, xnn_initialize(/*allocator=*/nullptr));

  xnn_operator_t op = nullptr;

  std::generate(input.begin(), input.end(), [&]() { return f32dist(rng); });
  std::generate(weights.begin(), weights.end(), [&]() { return f32dist(rng); });
  std::fill(operator_output.begin(), operator_output.end(), nanf(""));
  std::fill(subgraph_output.begin(), subgraph_output.end(), nanf(""));

  const xnn_status status = xnn_create_rope_nthc_f32(max_sequence_size, channels, weights.data(), /*flags=*/0, &op);
  if (status == xnn_status_unsupported_hardware) {
    GTEST_SKIP();
  }
  ASSERT_EQ(xnn_status_success, status);
  ASSERT_NE(nullptr, op);

  std::unique_ptr<xnn_operator, decltype(&xnn_delete_operator)> auto_op(op, xnn_delete_operator);

  ASSERT_EQ(xnn_status_success,
    xnn_reshape_rope_nthc_f32(op,
      batch_size, sequence_size, heads,
      /*threadpool=*/nullptr));

  ASSERT_EQ(xnn_status_success,
    xnn_setup_rope_nthc_f32(op,
      input.data(), operator_output.data()));

  ASSERT_EQ(xnn_status_success, xnn_run_operator(op, /*threadpool=*/nullptr));

  // Call subgraph API.
  xnn_subgraph_t subgraph = nullptr;
  ASSERT_EQ(xnn_status_success, xnn_create_subgraph(3, /*flags=*/0, &subgraph));
  std::unique_ptr<xnn_subgraph, decltype(&xnn_delete_subgraph)> auto_subgraph(subgraph, xnn_delete_subgraph);

  uint32_t input_id = XNN_INVALID_NODE_ID;
  const std::array<size_t, 4> input_dims{{batch_size, sequence_size, heads, channels}};
  ASSERT_EQ(xnn_status_success,
    xnn_define_tensor_value(subgraph, xnn_datatype_fp32, input_dims.size(), input_dims.data(),
                            /*data=*/nullptr, /*external_id=*/0, /*flags=*/XNN_VALUE_FLAG_EXTERNAL_INPUT, &input_id));
  ASSERT_NE(input_id, XNN_INVALID_NODE_ID);

  uint32_t weights_id = XNN_INVALID_NODE_ID;
  const std::array<size_t, 2> weights_dims{{max_sequence_size, channels}};
  ASSERT_EQ(xnn_status_success,
    xnn_define_tensor_value(subgraph, xnn_datatype_fp32, weights_dims.size(), weights_dims.data(),
                            weights.data(), /*external_id=*/1, /*flags=*/0, &weights_id));

  uint32_t output_id = XNN_INVALID_NODE_ID;
  const std::array<size_t, 4> output_dims{{batch_size, sequence_size, heads, channels}};
  ASSERT_EQ(
    xnn_status_success, xnn_define_tensor_value(
                          subgraph, xnn_datatype_fp32, output_dims.size(), output_dims.data(),
                          /*data=*/nullptr, /*external_id=*/2, /*flags=*/XNN_VALUE_FLAG_EXTERNAL_OUTPUT, &output_id));
  ASSERT_NE(output_id, XNN_INVALID_NODE_ID);

  ASSERT_EQ(xnn_status_success,
    xnn_define_rope(subgraph, max_sequence_size, input_id, weights_id, output_id, /*flags=*/0));

  xnn_runtime_t runtime = nullptr;
  ASSERT_EQ(xnn_status_success, xnn_create_runtime_v3(subgraph, nullptr, nullptr, /*flags=*/0, &runtime));
  ASSERT_NE(nullptr, runtime);

  std::unique_ptr<xnn_runtime, decltype(&xnn_delete_runtime)> auto_runtime(runtime, xnn_delete_runtime);

  const std::array<xnn_external_value, 2> external{{
    xnn_external_value{input_id, input.data()},
    xnn_external_value{output_id, subgraph_output.data()}
  }};
  ASSERT_EQ(xnn_status_success, xnn_setup_runtime(runtime, external.size(), external.data()));

  ASSERT_EQ(xnn_status_success, xnn_invoke_runtime(runtime));

  // Check outputs match.
  for (size_t i = 0; i < operator_output.size(); i++) {
    ASSERT_EQ(subgraph_output[i], operator_output[i]);
  }
}
// Copyright 2024 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <algorithm>
#include <cmath>
#include <functional>
#include <random>
#include <vector>

#include <benchmark/benchmark.h>
#include "bench/utils.h"

#include <xnnpack.h>
#include <xnnpack/aligned-allocator.h>
#include <xnnpack/common.h>
#include <xnnpack/microfnptr.h>
#include <xnnpack/microparams-init.h>
#include <xnnpack/vunary.h>


static void f32_vclamp(
  benchmark::State& state,
  xnn_f32_vclamp_ukernel_fn vclamp,
  xnn_init_f32_minmax_params_fn init_params = nullptr,
  benchmark::utils::IsaCheckFunction isa_check = nullptr)
{
  if (isa_check != nullptr && !isa_check(state)) {
    return;
  }

  const size_t num_elements = state.range(0);
  std::vector<float, AlignedAllocator<float, 64>> input(num_elements);
  std::vector<float, AlignedAllocator<float, 64>> output(num_elements);

  std::random_device random_device;
  auto rng = std::mt19937(random_device());
  auto f32rng = std::bind(std::uniform_real_distribution<float>(0.0f, 10.0f), std::ref(rng));
  std::generate(input.begin(), input.end(), std::ref(f32rng));
  std::fill(output.begin(), output.end(), std::nanf(""));

  union xnn_f32_minmax_params params;
  if (init_params != nullptr) {
    init_params(&params, -INFINITY, INFINITY);
  }
  for (auto _ : state) {
    vclamp(num_elements * sizeof(float), input.data(), output.data(), &params);
  }

  const uint64_t cpu_frequency = benchmark::utils::GetCurrentCpuFrequency();
  if (cpu_frequency != 0) {
    state.counters["cpufreq"] = cpu_frequency;
  }

  const size_t elements_per_iteration = num_elements;
  state.counters["elements"] =
    benchmark::Counter(uint64_t(state.iterations()) * elements_per_iteration, benchmark::Counter::kIsRate);

  const size_t bytes_per_iteration = 2 * num_elements * sizeof(float);
  state.counters["bytes"] =
    benchmark::Counter(uint64_t(state.iterations()) * bytes_per_iteration, benchmark::Counter::kIsRate);
}

#if XNN_ARCH_ARM || XNN_ARCH_ARM64
  BENCHMARK_CAPTURE(f32_vclamp, neon_u4,
                    xnn_f32_vclamp_ukernel__neon_u4,
                    xnn_init_f32_minmax_scalar_params,
                    benchmark::utils::CheckNEON)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<float, float>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f32_vclamp, neon_u8,
                    xnn_f32_vclamp_ukernel__neon_u8,
                    xnn_init_f32_minmax_scalar_params,
                    benchmark::utils::CheckNEON)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<float, float>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f32_vclamp, neon_u16,
                    xnn_f32_vclamp_ukernel__neon_u16,
                    xnn_init_f32_minmax_scalar_params,
                    benchmark::utils::CheckNEON)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<float, float>)
    ->UseRealTime();
#endif  // XNN_ARCH_ARM || XNN_ARCH_ARM64

#if XNN_ENABLE_RISCV_VECTOR && XNN_ARCH_RISCV
  BENCHMARK_CAPTURE(f32_vclamp, rvv_u1v,
                    xnn_f32_vclamp_ukernel__rvv_u1v,
                    xnn_init_f32_minmax_scalar_params,
                    benchmark::utils::CheckRVV)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<float, float>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f32_vclamp, rvv_u2v,
                    xnn_f32_vclamp_ukernel__rvv_u2v,
                    xnn_init_f32_minmax_scalar_params,
                    benchmark::utils::CheckRVV)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<float, float>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f32_vclamp, rvv_u4v,
                    xnn_f32_vclamp_ukernel__rvv_u4v,
                    xnn_init_f32_minmax_scalar_params,
                    benchmark::utils::CheckRVV)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<float, float>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f32_vclamp, rvv_u8v,
                    xnn_f32_vclamp_ukernel__rvv_u8v,
                    xnn_init_f32_minmax_scalar_params,
                    benchmark::utils::CheckRVV)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<float, float>)
    ->UseRealTime();
#endif  // XNN_ENABLE_RISCV_VECTOR && XNN_ARCH_RISCV

#if XNN_ARCH_X86 || XNN_ARCH_X86_64
  BENCHMARK_CAPTURE(f32_vclamp, avx512f_u16,
                    xnn_f32_vclamp_ukernel__avx512f_u16,
                    xnn_init_f32_minmax_scalar_params,
                    benchmark::utils::CheckAVX512F)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<float, float>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f32_vclamp, avx512f_u32,
                    xnn_f32_vclamp_ukernel__avx512f_u32,
                    xnn_init_f32_minmax_scalar_params,
                    benchmark::utils::CheckAVX512F)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<float, float>)
    ->UseRealTime();

  BENCHMARK_CAPTURE(f32_vclamp, avx_u8,
                    xnn_f32_vclamp_ukernel__avx_u8,
                    xnn_init_f32_minmax_avx_params,
                    benchmark::utils::CheckAVX)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<float, float>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f32_vclamp, avx_u16,
                    xnn_f32_vclamp_ukernel__avx_u16,
                    xnn_init_f32_minmax_avx_params,
                    benchmark::utils::CheckAVX)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<float, float>)
    ->UseRealTime();

  BENCHMARK_CAPTURE(f32_vclamp, sse_u4,
                    xnn_f32_vclamp_ukernel__sse_u4,
                    xnn_init_f32_minmax_sse_params)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<float, float>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f32_vclamp, sse_u8,
                    xnn_f32_vclamp_ukernel__sse_u8,
                    xnn_init_f32_minmax_sse_params)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<float, float>)
    ->UseRealTime();
#endif  // XNN_ARCH_X86 || XNN_ARCH_X86_64

#if XNN_ARCH_WASMSIMD || XNN_ARCH_WASMRELAXEDSIMD
  BENCHMARK_CAPTURE(f32_vclamp, wasmsimd_arm_u4,
                    xnn_f32_vclamp_ukernel__wasmsimd_arm_u4,
                    xnn_init_f32_minmax_wasmsimd_params)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<float, float>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f32_vclamp, wasmsimd_arm_u8,
                    xnn_f32_vclamp_ukernel__wasmsimd_arm_u8,
                    xnn_init_f32_minmax_wasmsimd_params)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<float, float>)
    ->UseRealTime();

  BENCHMARK_CAPTURE(f32_vclamp, wasmsimd_x86_u4,
                    xnn_f32_vclamp_ukernel__wasmsimd_x86_u4,
                    xnn_init_f32_minmax_wasmsimd_params)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<float, float>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f32_vclamp, wasmsimd_x86_u8,
                    xnn_f32_vclamp_ukernel__wasmsimd_x86_u8,
                    xnn_init_f32_minmax_wasmsimd_params)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<float, float>)
    ->UseRealTime();
#endif  // XNN_ARCH_WASMSIMD || XNN_ARCH_WASMRELAXEDSIMD

BENCHMARK_CAPTURE(f32_vclamp, scalar_u1,
                  xnn_f32_vclamp_ukernel__scalar_u1,
                  xnn_init_f32_minmax_scalar_params)
  ->Apply(benchmark::utils::UnaryElementwiseParameters<float, float>)
  ->UseRealTime();
BENCHMARK_CAPTURE(f32_vclamp, scalar_u2,
                  xnn_f32_vclamp_ukernel__scalar_u2,
                  xnn_init_f32_minmax_scalar_params)
  ->Apply(benchmark::utils::UnaryElementwiseParameters<float, float>)
  ->UseRealTime();
BENCHMARK_CAPTURE(f32_vclamp, scalar_u4,
                  xnn_f32_vclamp_ukernel__scalar_u4,
                  xnn_init_f32_minmax_scalar_params)
  ->Apply(benchmark::utils::UnaryElementwiseParameters<float, float>)
  ->UseRealTime();

#ifndef XNNPACK_BENCHMARK_NO_MAIN
BENCHMARK_MAIN();
#endif
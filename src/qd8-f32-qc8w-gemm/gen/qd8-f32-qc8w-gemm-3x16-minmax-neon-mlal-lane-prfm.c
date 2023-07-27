// Auto-generated file. Do not edit!
//   Template: src/qs8-gemm/neon-mlal-lane.c.in
//   Generator: tools/xngen
//
// Copyright 2020 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <arm_neon.h>

#include <xnnpack/common.h>
#include <xnnpack/gemm.h>
#include <xnnpack/prefetch.h>


void xnn_qd8_f32_qc8w_gemm_minmax_ukernel_3x16__neon_mlal_lane_prfm(
    size_t mr,
    size_t nc,
    size_t kc,
    const int8_t* restrict a,
    size_t a_stride,
    const void* restrict w,
    float* restrict c,
    size_t cm_stride,
    size_t cn_stride,
    const union xnn_f32_minmax_params params[restrict XNN_MIN_ELEMENTS(1)],
    const struct xnn_qd8_quantization_params quantization_params[restrict XNN_MIN_ELEMENTS(1)]) XNN_OOB_READS
{
  assert(mr != 0);
  assert(mr <= 3);
  assert(nc != 0);
  assert(kc != 0);
  assert(kc % sizeof(int8_t) == 0);
  assert(a != NULL);
  assert(w != NULL);
  assert(c != NULL);

  const int8_t* a0 = a;
  float* c0 = c;
  const int8_t* a1 = (const int8_t*) ((uintptr_t) a0 + a_stride);
  float* c1 = (float*) ((uintptr_t) c0 + cm_stride);
  if XNN_UNPREDICTABLE(mr < 2) {
    a1 = a0;
    c1 = c0;
  }
  const int8_t* a2 = (const int8_t*) ((uintptr_t) a1 + a_stride);
  float* c2 = (float*) ((uintptr_t) c1 + cm_stride);
  if XNN_UNPREDICTABLE(mr <= 2) {
    a2 = a1;
    c2 = c1;
  }

  do {
    const int32x4_t vksum0123 = vld1q_s32(w); w = (const int32_t*) w + 4;
    const int32x4_t vksum4567 = vld1q_s32(w); w = (const int32_t*) w + 4;
    const int32x4_t vksum89AB = vld1q_s32(w); w = (const int32_t*) w + 4;
    const int32x4_t vksumCDEF = vld1q_s32(w); w = (const int32_t*) w + 4;

    const int32x4_t vzp01 = vld1q_s32(&quantization_params[0].zero_point);
    int32x4_t vacc0x0123 = vmulq_lane_s32(vksum0123, vget_low_s32(vzp01), 0);
    int32x4_t vacc1x0123 = vmulq_lane_s32(vksum0123, vget_high_s32(vzp01), 0);
    int32x4_t vacc0x4567 = vmulq_lane_s32(vksum4567, vget_low_s32(vzp01), 0);
    int32x4_t vacc1x4567 = vmulq_lane_s32(vksum4567, vget_high_s32(vzp01), 0);
    int32x4_t vacc0x89AB = vmulq_lane_s32(vksum89AB, vget_low_s32(vzp01), 0);
    int32x4_t vacc1x89AB = vmulq_lane_s32(vksum89AB, vget_high_s32(vzp01), 0);
    int32x4_t vacc0xCDEF = vmulq_lane_s32(vksumCDEF, vget_low_s32(vzp01), 0);
    int32x4_t vacc1xCDEF = vmulq_lane_s32(vksumCDEF, vget_high_s32(vzp01), 0);
    const int32x4_t vzp2 = vld1q_dup_s32(&quantization_params[2].zero_point);
    int32x4_t vacc2x0123 = vmulq_s32(vksum0123, vzp2);
    int32x4_t vacc2x4567 = vmulq_s32(vksum4567, vzp2);
    int32x4_t vacc2x89AB = vmulq_s32(vksum89AB, vzp2);
    int32x4_t vacc2xCDEF = vmulq_s32(vksumCDEF, vzp2);

    size_t k = kc;
    while (k >= 8 * sizeof(int8_t)) {
      const int8x8_t va0 = vld1_s8(a0); a0 += 8;
      const int16x8_t vxa0 = vmovl_s8(va0);
      const int8x8_t va1 = vld1_s8(a1); a1 += 8;
      const int16x8_t vxa1 = vmovl_s8(va1);
      const int8x8_t va2 = vld1_s8(a2); a2 += 8;
      const int16x8_t vxa2 = vmovl_s8(va2);

      const int8x8_t vb01234567c0 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
      const int16x8_t vxb01234567c0 = vmovl_s8(vb01234567c0);

      vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vxb01234567c0), vget_low_s16(vxa0), 0);
      vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vxb01234567c0), vget_low_s16(vxa0), 0);
      vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vxb01234567c0), vget_low_s16(vxa1), 0);
      vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vxb01234567c0), vget_low_s16(vxa1), 0);
      vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vxb01234567c0), vget_low_s16(vxa2), 0);
      vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vxb01234567c0), vget_low_s16(vxa2), 0);
      const int8x8_t vb89ABCDEFc0 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
      const int16x8_t vxb89ABCDEFc0 = vmovl_s8(vb89ABCDEFc0);

      vacc0x89AB = vmlal_lane_s16(vacc0x89AB, vget_low_s16(vxb89ABCDEFc0), vget_low_s16(vxa0), 0);
      vacc0xCDEF = vmlal_lane_s16(vacc0xCDEF, vget_high_s16(vxb89ABCDEFc0), vget_low_s16(vxa0), 0);
      vacc1x89AB = vmlal_lane_s16(vacc1x89AB, vget_low_s16(vxb89ABCDEFc0), vget_low_s16(vxa1), 0);
      vacc1xCDEF = vmlal_lane_s16(vacc1xCDEF, vget_high_s16(vxb89ABCDEFc0), vget_low_s16(vxa1), 0);
      vacc2x89AB = vmlal_lane_s16(vacc2x89AB, vget_low_s16(vxb89ABCDEFc0), vget_low_s16(vxa2), 0);
      vacc2xCDEF = vmlal_lane_s16(vacc2xCDEF, vget_high_s16(vxb89ABCDEFc0), vget_low_s16(vxa2), 0);
      const int8x8_t vb01234567c1 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
      const int16x8_t vxb01234567c1 = vmovl_s8(vb01234567c1);

      vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vxb01234567c1), vget_low_s16(vxa0), 1);
      vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vxb01234567c1), vget_low_s16(vxa0), 1);
      vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vxb01234567c1), vget_low_s16(vxa1), 1);
      vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vxb01234567c1), vget_low_s16(vxa1), 1);
      vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vxb01234567c1), vget_low_s16(vxa2), 1);
      vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vxb01234567c1), vget_low_s16(vxa2), 1);
      const int8x8_t vb89ABCDEFc1 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
      const int16x8_t vxb89ABCDEFc1 = vmovl_s8(vb89ABCDEFc1);

      vacc0x89AB = vmlal_lane_s16(vacc0x89AB, vget_low_s16(vxb89ABCDEFc1), vget_low_s16(vxa0), 1);
      vacc0xCDEF = vmlal_lane_s16(vacc0xCDEF, vget_high_s16(vxb89ABCDEFc1), vget_low_s16(vxa0), 1);
      vacc1x89AB = vmlal_lane_s16(vacc1x89AB, vget_low_s16(vxb89ABCDEFc1), vget_low_s16(vxa1), 1);
      vacc1xCDEF = vmlal_lane_s16(vacc1xCDEF, vget_high_s16(vxb89ABCDEFc1), vget_low_s16(vxa1), 1);
      vacc2x89AB = vmlal_lane_s16(vacc2x89AB, vget_low_s16(vxb89ABCDEFc1), vget_low_s16(vxa2), 1);
      vacc2xCDEF = vmlal_lane_s16(vacc2xCDEF, vget_high_s16(vxb89ABCDEFc1), vget_low_s16(vxa2), 1);
      const int8x8_t vb01234567c2 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
      const int16x8_t vxb01234567c2 = vmovl_s8(vb01234567c2);

      vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vxb01234567c2), vget_low_s16(vxa0), 2);
      vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vxb01234567c2), vget_low_s16(vxa0), 2);
      vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vxb01234567c2), vget_low_s16(vxa1), 2);
      vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vxb01234567c2), vget_low_s16(vxa1), 2);
      vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vxb01234567c2), vget_low_s16(vxa2), 2);
      vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vxb01234567c2), vget_low_s16(vxa2), 2);
      const int8x8_t vb89ABCDEFc2 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
      const int16x8_t vxb89ABCDEFc2 = vmovl_s8(vb89ABCDEFc2);

      vacc0x89AB = vmlal_lane_s16(vacc0x89AB, vget_low_s16(vxb89ABCDEFc2), vget_low_s16(vxa0), 2);
      vacc0xCDEF = vmlal_lane_s16(vacc0xCDEF, vget_high_s16(vxb89ABCDEFc2), vget_low_s16(vxa0), 2);
      vacc1x89AB = vmlal_lane_s16(vacc1x89AB, vget_low_s16(vxb89ABCDEFc2), vget_low_s16(vxa1), 2);
      vacc1xCDEF = vmlal_lane_s16(vacc1xCDEF, vget_high_s16(vxb89ABCDEFc2), vget_low_s16(vxa1), 2);
      vacc2x89AB = vmlal_lane_s16(vacc2x89AB, vget_low_s16(vxb89ABCDEFc2), vget_low_s16(vxa2), 2);
      vacc2xCDEF = vmlal_lane_s16(vacc2xCDEF, vget_high_s16(vxb89ABCDEFc2), vget_low_s16(vxa2), 2);
      const int8x8_t vb01234567c3 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
      const int16x8_t vxb01234567c3 = vmovl_s8(vb01234567c3);

      vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vxb01234567c3), vget_low_s16(vxa0), 3);
      vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vxb01234567c3), vget_low_s16(vxa0), 3);
      vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vxb01234567c3), vget_low_s16(vxa1), 3);
      vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vxb01234567c3), vget_low_s16(vxa1), 3);
      vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vxb01234567c3), vget_low_s16(vxa2), 3);
      vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vxb01234567c3), vget_low_s16(vxa2), 3);
      const int8x8_t vb89ABCDEFc3 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
      const int16x8_t vxb89ABCDEFc3 = vmovl_s8(vb89ABCDEFc3);

      vacc0x89AB = vmlal_lane_s16(vacc0x89AB, vget_low_s16(vxb89ABCDEFc3), vget_low_s16(vxa0), 3);
      vacc0xCDEF = vmlal_lane_s16(vacc0xCDEF, vget_high_s16(vxb89ABCDEFc3), vget_low_s16(vxa0), 3);
      vacc1x89AB = vmlal_lane_s16(vacc1x89AB, vget_low_s16(vxb89ABCDEFc3), vget_low_s16(vxa1), 3);
      vacc1xCDEF = vmlal_lane_s16(vacc1xCDEF, vget_high_s16(vxb89ABCDEFc3), vget_low_s16(vxa1), 3);
      vacc2x89AB = vmlal_lane_s16(vacc2x89AB, vget_low_s16(vxb89ABCDEFc3), vget_low_s16(vxa2), 3);
      vacc2xCDEF = vmlal_lane_s16(vacc2xCDEF, vget_high_s16(vxb89ABCDEFc3), vget_low_s16(vxa2), 3);

      xnn_prefetch_to_l1((const int8_t*) w + 448);
      xnn_prefetch_to_l1((const int8_t*) w + 512);

      const int8x8_t vb01234567c4 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
      const int16x8_t vxb01234567c4 = vmovl_s8(vb01234567c4);

      vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vxb01234567c4), vget_high_s16(vxa0), 0);
      vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vxb01234567c4), vget_high_s16(vxa0), 0);
      vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vxb01234567c4), vget_high_s16(vxa1), 0);
      vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vxb01234567c4), vget_high_s16(vxa1), 0);
      vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vxb01234567c4), vget_high_s16(vxa2), 0);
      vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vxb01234567c4), vget_high_s16(vxa2), 0);
      const int8x8_t vb89ABCDEFc4 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
      const int16x8_t vxb89ABCDEFc4 = vmovl_s8(vb89ABCDEFc4);

      vacc0x89AB = vmlal_lane_s16(vacc0x89AB, vget_low_s16(vxb89ABCDEFc4), vget_high_s16(vxa0), 0);
      vacc0xCDEF = vmlal_lane_s16(vacc0xCDEF, vget_high_s16(vxb89ABCDEFc4), vget_high_s16(vxa0), 0);
      vacc1x89AB = vmlal_lane_s16(vacc1x89AB, vget_low_s16(vxb89ABCDEFc4), vget_high_s16(vxa1), 0);
      vacc1xCDEF = vmlal_lane_s16(vacc1xCDEF, vget_high_s16(vxb89ABCDEFc4), vget_high_s16(vxa1), 0);
      vacc2x89AB = vmlal_lane_s16(vacc2x89AB, vget_low_s16(vxb89ABCDEFc4), vget_high_s16(vxa2), 0);
      vacc2xCDEF = vmlal_lane_s16(vacc2xCDEF, vget_high_s16(vxb89ABCDEFc4), vget_high_s16(vxa2), 0);
      const int8x8_t vb01234567c5 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
      const int16x8_t vxb01234567c5 = vmovl_s8(vb01234567c5);

      vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vxb01234567c5), vget_high_s16(vxa0), 1);
      vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vxb01234567c5), vget_high_s16(vxa0), 1);
      vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vxb01234567c5), vget_high_s16(vxa1), 1);
      vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vxb01234567c5), vget_high_s16(vxa1), 1);
      vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vxb01234567c5), vget_high_s16(vxa2), 1);
      vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vxb01234567c5), vget_high_s16(vxa2), 1);
      const int8x8_t vb89ABCDEFc5 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
      const int16x8_t vxb89ABCDEFc5 = vmovl_s8(vb89ABCDEFc5);

      vacc0x89AB = vmlal_lane_s16(vacc0x89AB, vget_low_s16(vxb89ABCDEFc5), vget_high_s16(vxa0), 1);
      vacc0xCDEF = vmlal_lane_s16(vacc0xCDEF, vget_high_s16(vxb89ABCDEFc5), vget_high_s16(vxa0), 1);
      vacc1x89AB = vmlal_lane_s16(vacc1x89AB, vget_low_s16(vxb89ABCDEFc5), vget_high_s16(vxa1), 1);
      vacc1xCDEF = vmlal_lane_s16(vacc1xCDEF, vget_high_s16(vxb89ABCDEFc5), vget_high_s16(vxa1), 1);
      vacc2x89AB = vmlal_lane_s16(vacc2x89AB, vget_low_s16(vxb89ABCDEFc5), vget_high_s16(vxa2), 1);
      vacc2xCDEF = vmlal_lane_s16(vacc2xCDEF, vget_high_s16(vxb89ABCDEFc5), vget_high_s16(vxa2), 1);
      const int8x8_t vb01234567c6 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
      const int16x8_t vxb01234567c6 = vmovl_s8(vb01234567c6);

      vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vxb01234567c6), vget_high_s16(vxa0), 2);
      vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vxb01234567c6), vget_high_s16(vxa0), 2);
      vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vxb01234567c6), vget_high_s16(vxa1), 2);
      vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vxb01234567c6), vget_high_s16(vxa1), 2);
      vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vxb01234567c6), vget_high_s16(vxa2), 2);
      vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vxb01234567c6), vget_high_s16(vxa2), 2);
      const int8x8_t vb89ABCDEFc6 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
      const int16x8_t vxb89ABCDEFc6 = vmovl_s8(vb89ABCDEFc6);

      vacc0x89AB = vmlal_lane_s16(vacc0x89AB, vget_low_s16(vxb89ABCDEFc6), vget_high_s16(vxa0), 2);
      vacc0xCDEF = vmlal_lane_s16(vacc0xCDEF, vget_high_s16(vxb89ABCDEFc6), vget_high_s16(vxa0), 2);
      vacc1x89AB = vmlal_lane_s16(vacc1x89AB, vget_low_s16(vxb89ABCDEFc6), vget_high_s16(vxa1), 2);
      vacc1xCDEF = vmlal_lane_s16(vacc1xCDEF, vget_high_s16(vxb89ABCDEFc6), vget_high_s16(vxa1), 2);
      vacc2x89AB = vmlal_lane_s16(vacc2x89AB, vget_low_s16(vxb89ABCDEFc6), vget_high_s16(vxa2), 2);
      vacc2xCDEF = vmlal_lane_s16(vacc2xCDEF, vget_high_s16(vxb89ABCDEFc6), vget_high_s16(vxa2), 2);
      const int8x8_t vb01234567c7 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
      const int16x8_t vxb01234567c7 = vmovl_s8(vb01234567c7);

      vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vxb01234567c7), vget_high_s16(vxa0), 3);
      vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vxb01234567c7), vget_high_s16(vxa0), 3);
      vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vxb01234567c7), vget_high_s16(vxa1), 3);
      vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vxb01234567c7), vget_high_s16(vxa1), 3);
      vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vxb01234567c7), vget_high_s16(vxa2), 3);
      vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vxb01234567c7), vget_high_s16(vxa2), 3);
      const int8x8_t vb89ABCDEFc7 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
      const int16x8_t vxb89ABCDEFc7 = vmovl_s8(vb89ABCDEFc7);

      vacc0x89AB = vmlal_lane_s16(vacc0x89AB, vget_low_s16(vxb89ABCDEFc7), vget_high_s16(vxa0), 3);
      vacc0xCDEF = vmlal_lane_s16(vacc0xCDEF, vget_high_s16(vxb89ABCDEFc7), vget_high_s16(vxa0), 3);
      vacc1x89AB = vmlal_lane_s16(vacc1x89AB, vget_low_s16(vxb89ABCDEFc7), vget_high_s16(vxa1), 3);
      vacc1xCDEF = vmlal_lane_s16(vacc1xCDEF, vget_high_s16(vxb89ABCDEFc7), vget_high_s16(vxa1), 3);
      vacc2x89AB = vmlal_lane_s16(vacc2x89AB, vget_low_s16(vxb89ABCDEFc7), vget_high_s16(vxa2), 3);
      vacc2xCDEF = vmlal_lane_s16(vacc2xCDEF, vget_high_s16(vxb89ABCDEFc7), vget_high_s16(vxa2), 3);

      k -= 8 * sizeof(int8_t);
    }
    if XNN_UNLIKELY(k != 0) {
      const int8x8_t va0 = vld1_s8(a0); a0 = (const int8_t*) ((uintptr_t) a0 + k);
      const int16x8_t vxa0 = vmovl_s8(va0);
      const int8x8_t va1 = vld1_s8(a1); a1 = (const int8_t*) ((uintptr_t) a1 + k);
      const int16x8_t vxa1 = vmovl_s8(va1);
      const int8x8_t va2 = vld1_s8(a2); a2 = (const int8_t*) ((uintptr_t) a2 + k);
      const int16x8_t vxa2 = vmovl_s8(va2);

      const int8x8_t vb01234567c0 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
      const int16x8_t vxb01234567c0 = vmovl_s8(vb01234567c0);
      const int8x8_t vb89ABCDEFc0 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
      const int16x8_t vxb89ABCDEFc0 = vmovl_s8(vb89ABCDEFc0);

      vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vxb01234567c0), vget_low_s16(vxa0), 0);
      vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vxb01234567c0), vget_low_s16(vxa0), 0);
      vacc0x89AB = vmlal_lane_s16(vacc0x89AB, vget_low_s16(vxb89ABCDEFc0), vget_low_s16(vxa0), 0);
      vacc0xCDEF = vmlal_lane_s16(vacc0xCDEF, vget_high_s16(vxb89ABCDEFc0), vget_low_s16(vxa0), 0);
      vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vxb01234567c0), vget_low_s16(vxa1), 0);
      vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vxb01234567c0), vget_low_s16(vxa1), 0);
      vacc1x89AB = vmlal_lane_s16(vacc1x89AB, vget_low_s16(vxb89ABCDEFc0), vget_low_s16(vxa1), 0);
      vacc1xCDEF = vmlal_lane_s16(vacc1xCDEF, vget_high_s16(vxb89ABCDEFc0), vget_low_s16(vxa1), 0);
      vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vxb01234567c0), vget_low_s16(vxa2), 0);
      vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vxb01234567c0), vget_low_s16(vxa2), 0);
      vacc2x89AB = vmlal_lane_s16(vacc2x89AB, vget_low_s16(vxb89ABCDEFc0), vget_low_s16(vxa2), 0);
      vacc2xCDEF = vmlal_lane_s16(vacc2xCDEF, vget_high_s16(vxb89ABCDEFc0), vget_low_s16(vxa2), 0);

      if (k >= 2 * sizeof(int8_t)) {
        const int8x8_t vb01234567c1 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
        const int16x8_t vxb01234567c1 = vmovl_s8(vb01234567c1);
        const int8x8_t vb89ABCDEFc1 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
        const int16x8_t vxb89ABCDEFc1 = vmovl_s8(vb89ABCDEFc1);

        vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vxb01234567c1), vget_low_s16(vxa0), 1);
        vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vxb01234567c1), vget_low_s16(vxa0), 1);
        vacc0x89AB = vmlal_lane_s16(vacc0x89AB, vget_low_s16(vxb89ABCDEFc1), vget_low_s16(vxa0), 1);
        vacc0xCDEF = vmlal_lane_s16(vacc0xCDEF, vget_high_s16(vxb89ABCDEFc1), vget_low_s16(vxa0), 1);
        vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vxb01234567c1), vget_low_s16(vxa1), 1);
        vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vxb01234567c1), vget_low_s16(vxa1), 1);
        vacc1x89AB = vmlal_lane_s16(vacc1x89AB, vget_low_s16(vxb89ABCDEFc1), vget_low_s16(vxa1), 1);
        vacc1xCDEF = vmlal_lane_s16(vacc1xCDEF, vget_high_s16(vxb89ABCDEFc1), vget_low_s16(vxa1), 1);
        vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vxb01234567c1), vget_low_s16(vxa2), 1);
        vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vxb01234567c1), vget_low_s16(vxa2), 1);
        vacc2x89AB = vmlal_lane_s16(vacc2x89AB, vget_low_s16(vxb89ABCDEFc1), vget_low_s16(vxa2), 1);
        vacc2xCDEF = vmlal_lane_s16(vacc2xCDEF, vget_high_s16(vxb89ABCDEFc1), vget_low_s16(vxa2), 1);

        if (k > 2 * sizeof(int8_t)) {
          const int8x8_t vb01234567c2 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
          const int16x8_t vxb01234567c2 = vmovl_s8(vb01234567c2);
          const int8x8_t vb89ABCDEFc2 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
          const int16x8_t vxb89ABCDEFc2 = vmovl_s8(vb89ABCDEFc2);

          vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vxb01234567c2), vget_low_s16(vxa0), 2);
          vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vxb01234567c2), vget_low_s16(vxa0), 2);
          vacc0x89AB = vmlal_lane_s16(vacc0x89AB, vget_low_s16(vxb89ABCDEFc2), vget_low_s16(vxa0), 2);
          vacc0xCDEF = vmlal_lane_s16(vacc0xCDEF, vget_high_s16(vxb89ABCDEFc2), vget_low_s16(vxa0), 2);
          vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vxb01234567c2), vget_low_s16(vxa1), 2);
          vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vxb01234567c2), vget_low_s16(vxa1), 2);
          vacc1x89AB = vmlal_lane_s16(vacc1x89AB, vget_low_s16(vxb89ABCDEFc2), vget_low_s16(vxa1), 2);
          vacc1xCDEF = vmlal_lane_s16(vacc1xCDEF, vget_high_s16(vxb89ABCDEFc2), vget_low_s16(vxa1), 2);
          vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vxb01234567c2), vget_low_s16(vxa2), 2);
          vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vxb01234567c2), vget_low_s16(vxa2), 2);
          vacc2x89AB = vmlal_lane_s16(vacc2x89AB, vget_low_s16(vxb89ABCDEFc2), vget_low_s16(vxa2), 2);
          vacc2xCDEF = vmlal_lane_s16(vacc2xCDEF, vget_high_s16(vxb89ABCDEFc2), vget_low_s16(vxa2), 2);

          if (k >= 4 * sizeof(int8_t)) {
            const int8x8_t vb01234567c3 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
            const int16x8_t vxb01234567c3 = vmovl_s8(vb01234567c3);
            const int8x8_t vb89ABCDEFc3 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
            const int16x8_t vxb89ABCDEFc3 = vmovl_s8(vb89ABCDEFc3);

            vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vxb01234567c3), vget_low_s16(vxa0), 3);
            vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vxb01234567c3), vget_low_s16(vxa0), 3);
            vacc0x89AB = vmlal_lane_s16(vacc0x89AB, vget_low_s16(vxb89ABCDEFc3), vget_low_s16(vxa0), 3);
            vacc0xCDEF = vmlal_lane_s16(vacc0xCDEF, vget_high_s16(vxb89ABCDEFc3), vget_low_s16(vxa0), 3);
            vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vxb01234567c3), vget_low_s16(vxa1), 3);
            vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vxb01234567c3), vget_low_s16(vxa1), 3);
            vacc1x89AB = vmlal_lane_s16(vacc1x89AB, vget_low_s16(vxb89ABCDEFc3), vget_low_s16(vxa1), 3);
            vacc1xCDEF = vmlal_lane_s16(vacc1xCDEF, vget_high_s16(vxb89ABCDEFc3), vget_low_s16(vxa1), 3);
            vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vxb01234567c3), vget_low_s16(vxa2), 3);
            vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vxb01234567c3), vget_low_s16(vxa2), 3);
            vacc2x89AB = vmlal_lane_s16(vacc2x89AB, vget_low_s16(vxb89ABCDEFc3), vget_low_s16(vxa2), 3);
            vacc2xCDEF = vmlal_lane_s16(vacc2xCDEF, vget_high_s16(vxb89ABCDEFc3), vget_low_s16(vxa2), 3);

            if (k > 4 * sizeof(int8_t)) {
              const int8x8_t vb01234567c4 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
              const int16x8_t vxb01234567c4 = vmovl_s8(vb01234567c4);
              const int8x8_t vb89ABCDEFc4 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
              const int16x8_t vxb89ABCDEFc4 = vmovl_s8(vb89ABCDEFc4);

              vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vxb01234567c4), vget_high_s16(vxa0), 0);
              vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vxb01234567c4), vget_high_s16(vxa0), 0);
              vacc0x89AB = vmlal_lane_s16(vacc0x89AB, vget_low_s16(vxb89ABCDEFc4), vget_high_s16(vxa0), 0);
              vacc0xCDEF = vmlal_lane_s16(vacc0xCDEF, vget_high_s16(vxb89ABCDEFc4), vget_high_s16(vxa0), 0);
              vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vxb01234567c4), vget_high_s16(vxa1), 0);
              vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vxb01234567c4), vget_high_s16(vxa1), 0);
              vacc1x89AB = vmlal_lane_s16(vacc1x89AB, vget_low_s16(vxb89ABCDEFc4), vget_high_s16(vxa1), 0);
              vacc1xCDEF = vmlal_lane_s16(vacc1xCDEF, vget_high_s16(vxb89ABCDEFc4), vget_high_s16(vxa1), 0);
              vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vxb01234567c4), vget_high_s16(vxa2), 0);
              vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vxb01234567c4), vget_high_s16(vxa2), 0);
              vacc2x89AB = vmlal_lane_s16(vacc2x89AB, vget_low_s16(vxb89ABCDEFc4), vget_high_s16(vxa2), 0);
              vacc2xCDEF = vmlal_lane_s16(vacc2xCDEF, vget_high_s16(vxb89ABCDEFc4), vget_high_s16(vxa2), 0);

              if (k >= 6 * sizeof(int8_t)) {
                const int8x8_t vb01234567c5 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
                const int16x8_t vxb01234567c5 = vmovl_s8(vb01234567c5);
                const int8x8_t vb89ABCDEFc5 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
                const int16x8_t vxb89ABCDEFc5 = vmovl_s8(vb89ABCDEFc5);

                vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vxb01234567c5), vget_high_s16(vxa0), 1);
                vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vxb01234567c5), vget_high_s16(vxa0), 1);
                vacc0x89AB = vmlal_lane_s16(vacc0x89AB, vget_low_s16(vxb89ABCDEFc5), vget_high_s16(vxa0), 1);
                vacc0xCDEF = vmlal_lane_s16(vacc0xCDEF, vget_high_s16(vxb89ABCDEFc5), vget_high_s16(vxa0), 1);
                vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vxb01234567c5), vget_high_s16(vxa1), 1);
                vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vxb01234567c5), vget_high_s16(vxa1), 1);
                vacc1x89AB = vmlal_lane_s16(vacc1x89AB, vget_low_s16(vxb89ABCDEFc5), vget_high_s16(vxa1), 1);
                vacc1xCDEF = vmlal_lane_s16(vacc1xCDEF, vget_high_s16(vxb89ABCDEFc5), vget_high_s16(vxa1), 1);
                vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vxb01234567c5), vget_high_s16(vxa2), 1);
                vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vxb01234567c5), vget_high_s16(vxa2), 1);
                vacc2x89AB = vmlal_lane_s16(vacc2x89AB, vget_low_s16(vxb89ABCDEFc5), vget_high_s16(vxa2), 1);
                vacc2xCDEF = vmlal_lane_s16(vacc2xCDEF, vget_high_s16(vxb89ABCDEFc5), vget_high_s16(vxa2), 1);

                if (k > 6 * sizeof(int8_t)) {
                  const int8x8_t vb01234567c6 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
                  const int16x8_t vxb01234567c6 = vmovl_s8(vb01234567c6);
                  const int8x8_t vb89ABCDEFc6 = vld1_s8(w); w = (const void*) ((const int8_t*) w + 8);
                  const int16x8_t vxb89ABCDEFc6 = vmovl_s8(vb89ABCDEFc6);

                  vacc0x0123 = vmlal_lane_s16(vacc0x0123, vget_low_s16(vxb01234567c6), vget_high_s16(vxa0), 2);
                  vacc0x4567 = vmlal_lane_s16(vacc0x4567, vget_high_s16(vxb01234567c6), vget_high_s16(vxa0), 2);
                  vacc0x89AB = vmlal_lane_s16(vacc0x89AB, vget_low_s16(vxb89ABCDEFc6), vget_high_s16(vxa0), 2);
                  vacc0xCDEF = vmlal_lane_s16(vacc0xCDEF, vget_high_s16(vxb89ABCDEFc6), vget_high_s16(vxa0), 2);
                  vacc1x0123 = vmlal_lane_s16(vacc1x0123, vget_low_s16(vxb01234567c6), vget_high_s16(vxa1), 2);
                  vacc1x4567 = vmlal_lane_s16(vacc1x4567, vget_high_s16(vxb01234567c6), vget_high_s16(vxa1), 2);
                  vacc1x89AB = vmlal_lane_s16(vacc1x89AB, vget_low_s16(vxb89ABCDEFc6), vget_high_s16(vxa1), 2);
                  vacc1xCDEF = vmlal_lane_s16(vacc1xCDEF, vget_high_s16(vxb89ABCDEFc6), vget_high_s16(vxa1), 2);
                  vacc2x0123 = vmlal_lane_s16(vacc2x0123, vget_low_s16(vxb01234567c6), vget_high_s16(vxa2), 2);
                  vacc2x4567 = vmlal_lane_s16(vacc2x4567, vget_high_s16(vxb01234567c6), vget_high_s16(vxa2), 2);
                  vacc2x89AB = vmlal_lane_s16(vacc2x89AB, vget_low_s16(vxb89ABCDEFc6), vget_high_s16(vxa2), 2);
                  vacc2xCDEF = vmlal_lane_s16(vacc2xCDEF, vget_high_s16(vxb89ABCDEFc6), vget_high_s16(vxa2), 2);
                }
              }
            }
          }
        }
      }
    }

    float32x4_t vout0x0123 = vcvtq_f32_s32(vacc0x0123);
    float32x4_t vout0x4567 = vcvtq_f32_s32(vacc0x4567);
    float32x4_t vout0x89AB = vcvtq_f32_s32(vacc0x89AB);
    float32x4_t vout0xCDEF = vcvtq_f32_s32(vacc0xCDEF);
    float32x4_t vout1x0123 = vcvtq_f32_s32(vacc1x0123);
    float32x4_t vout1x4567 = vcvtq_f32_s32(vacc1x4567);
    float32x4_t vout1x89AB = vcvtq_f32_s32(vacc1x89AB);
    float32x4_t vout1xCDEF = vcvtq_f32_s32(vacc1xCDEF);
    float32x4_t vout2x0123 = vcvtq_f32_s32(vacc2x0123);
    float32x4_t vout2x4567 = vcvtq_f32_s32(vacc2x4567);
    float32x4_t vout2x89AB = vcvtq_f32_s32(vacc2x89AB);
    float32x4_t vout2xCDEF = vcvtq_f32_s32(vacc2xCDEF);
    const float32x4_t vbscale0123 = vld1q_f32(w); w = (const float*) w + 4;
    const float32x4_t vbscale4567 = vld1q_f32(w); w = (const float*) w + 4;
    const float32x4_t vbscale89AB = vld1q_f32(w); w = (const float*) w + 4;
    const float32x4_t vbscaleCDEF = vld1q_f32(w); w = (const float*) w + 4;
    const float32x4_t vinput_scale01 = vreinterpretq_f32_s32(vld1q_s32(&quantization_params[0].zero_point));
    const float32x4_t vscale0x0123 = vmulq_lane_f32(vbscale0123, vget_low_f32(vinput_scale01), 1);
    const float32x4_t vscale1x0123 = vmulq_lane_f32(vbscale0123, vget_high_f32(vinput_scale01), 1);
    const float32x4_t vscale0x4567 = vmulq_lane_f32(vbscale4567, vget_low_f32(vinput_scale01), 1);
    const float32x4_t vscale1x4567 = vmulq_lane_f32(vbscale4567, vget_high_f32(vinput_scale01), 1);
    const float32x4_t vscale0x89AB = vmulq_lane_f32(vbscale89AB, vget_low_f32(vinput_scale01), 1);
    const float32x4_t vscale1x89AB = vmulq_lane_f32(vbscale89AB, vget_high_f32(vinput_scale01), 1);
    const float32x4_t vscale0xCDEF = vmulq_lane_f32(vbscaleCDEF, vget_low_f32(vinput_scale01), 1);
    const float32x4_t vscale1xCDEF = vmulq_lane_f32(vbscaleCDEF, vget_high_f32(vinput_scale01), 1);
    const float32x4_t vbias0123 = vld1q_f32(w); w = (const float*) w + 4;
    const float32x4_t vbias4567 = vld1q_f32(w); w = (const float*) w + 4;
    const float32x4_t vbias89AB = vld1q_f32(w); w = (const float*) w + 4;
    const float32x4_t vbiasCDEF = vld1q_f32(w); w = (const float*) w + 4;
    #if XNN_ARCH_ARM64
      vout0x0123 = vfmaq_f32(vbias0123, vout0x0123, vscale0x0123);
      vout1x0123 = vfmaq_f32(vbias0123, vout1x0123, vscale1x0123);
      vout0x4567 = vfmaq_f32(vbias4567, vout0x4567, vscale0x4567);
      vout1x4567 = vfmaq_f32(vbias4567, vout1x4567, vscale1x4567);
      vout0x89AB = vfmaq_f32(vbias89AB, vout0x89AB, vscale0x89AB);
      vout1x89AB = vfmaq_f32(vbias89AB, vout1x89AB, vscale1x89AB);
      vout0xCDEF = vfmaq_f32(vbiasCDEF, vout0xCDEF, vscale0xCDEF);
      vout1xCDEF = vfmaq_f32(vbiasCDEF, vout1xCDEF, vscale1xCDEF);
    #else
      vout0x0123 = vmlaq_f32(vbias0123, vout0x0123, vscale0x0123);
      vout1x0123 = vmlaq_f32(vbias0123, vout1x0123, vscale1x0123);
      vout0x4567 = vmlaq_f32(vbias4567, vout0x4567, vscale0x4567);
      vout1x4567 = vmlaq_f32(vbias4567, vout1x4567, vscale1x4567);
      vout0x89AB = vmlaq_f32(vbias89AB, vout0x89AB, vscale0x89AB);
      vout1x89AB = vmlaq_f32(vbias89AB, vout1x89AB, vscale1x89AB);
      vout0xCDEF = vmlaq_f32(vbiasCDEF, vout0xCDEF, vscale0xCDEF);
      vout1xCDEF = vmlaq_f32(vbiasCDEF, vout1xCDEF, vscale1xCDEF);
    #endif
    const float32x4_t vinput_scale2 = vld1q_dup_f32(&quantization_params[2].inv_scale);
    const float32x4_t vscale2x0123 = vmulq_f32(vbscale0123, vinput_scale2);
    const float32x4_t vscale2x4567 = vmulq_f32(vbscale4567, vinput_scale2);
    const float32x4_t vscale2x89AB = vmulq_f32(vbscale89AB, vinput_scale2);
    const float32x4_t vscale2xCDEF = vmulq_f32(vbscaleCDEF, vinput_scale2);
    #if XNN_ARCH_ARM64
      vout2x0123 = vfmaq_f32(vbias0123, vout2x0123, vscale2x0123);
      vout2x4567 = vfmaq_f32(vbias4567, vout2x4567, vscale2x4567);
      vout2x89AB = vfmaq_f32(vbias89AB, vout2x89AB, vscale2x89AB);
      vout2xCDEF = vfmaq_f32(vbiasCDEF, vout2xCDEF, vscale2xCDEF);
    #else
      vout2x0123 = vmlaq_f32(vbias0123, vout2x0123, vscale2x0123);
      vout2x4567 = vmlaq_f32(vbias4567, vout2x4567, vscale2x4567);
      vout2x89AB = vmlaq_f32(vbias89AB, vout2x89AB, vscale2x89AB);
      vout2xCDEF = vmlaq_f32(vbiasCDEF, vout2xCDEF, vscale2xCDEF);
    #endif

    const float32x4_t vmin = vld1q_dup_f32(&params->scalar.min);
    const float32x4_t vmax = vld1q_dup_f32(&params->scalar.max);
    vout0x0123 = vmaxq_f32(vout0x0123, vmin);
    vout0x4567 = vmaxq_f32(vout0x4567, vmin);
    vout0x89AB = vmaxq_f32(vout0x89AB, vmin);
    vout0xCDEF = vmaxq_f32(vout0xCDEF, vmin);
    vout0x0123 = vminq_f32(vout0x0123, vmax);
    vout0x4567 = vminq_f32(vout0x4567, vmax);
    vout0x89AB = vminq_f32(vout0x89AB, vmax);
    vout0xCDEF = vminq_f32(vout0xCDEF, vmax);
    vout1x0123 = vmaxq_f32(vout1x0123, vmin);
    vout1x4567 = vmaxq_f32(vout1x4567, vmin);
    vout1x89AB = vmaxq_f32(vout1x89AB, vmin);
    vout1xCDEF = vmaxq_f32(vout1xCDEF, vmin);
    vout1x0123 = vminq_f32(vout1x0123, vmax);
    vout1x4567 = vminq_f32(vout1x4567, vmax);
    vout1x89AB = vminq_f32(vout1x89AB, vmax);
    vout1xCDEF = vminq_f32(vout1xCDEF, vmax);
    vout2x0123 = vmaxq_f32(vout2x0123, vmin);
    vout2x4567 = vmaxq_f32(vout2x4567, vmin);
    vout2x89AB = vmaxq_f32(vout2x89AB, vmin);
    vout2xCDEF = vmaxq_f32(vout2xCDEF, vmin);
    vout2x0123 = vminq_f32(vout2x0123, vmax);
    vout2x4567 = vminq_f32(vout2x4567, vmax);
    vout2x89AB = vminq_f32(vout2x89AB, vmax);
    vout2xCDEF = vminq_f32(vout2xCDEF, vmax);
    if XNN_LIKELY(nc >= 16) {
      vst1q_f32(&c2[0], vout2x0123);
      vst1q_f32(&c2[4], vout2x4567);
      vst1q_f32(&c2[8], vout2x89AB);
      vst1q_f32(&c2[12], vout2xCDEF);
      vst1q_f32(&c1[0], vout1x0123);
      vst1q_f32(&c1[4], vout1x4567);
      vst1q_f32(&c1[8], vout1x89AB);
      vst1q_f32(&c1[12], vout1xCDEF);
      vst1q_f32(&c0[0], vout0x0123);
      vst1q_f32(&c0[4], vout0x4567);
      vst1q_f32(&c0[8], vout0x89AB);
      vst1q_f32(&c0[12], vout0xCDEF);

      a0 = (const int8_t*) ((uintptr_t) a0 - kc);
      a1 = (const int8_t*) ((uintptr_t) a1 - kc);
      a2 = (const int8_t*) ((uintptr_t) a2 - kc);

      c0 = (float*) ((uintptr_t) c0 + cn_stride);
      c1 = (float*) ((uintptr_t) c1 + cn_stride);
      c2 = (float*) ((uintptr_t) c2 + cn_stride);

      nc -= 16;
    } else {
      if (nc & 8) {
        vst1q_f32(c2, vout2x0123); c2 += 4;
        vout2x0123 = vout2x89AB;
        vst1q_f32(c1, vout1x0123); c1 += 4;
        vout1x0123 = vout1x89AB;
        vst1q_f32(c0, vout0x0123); c0 += 4;
        vout0x0123 = vout0x89AB;
        vst1q_f32(c2, vout2x4567); c2 += 4;
        vout2x4567 = vout2xCDEF;
        vst1q_f32(c1, vout1x4567); c1 += 4;
        vout1x4567 = vout1xCDEF;
        vst1q_f32(c0, vout0x4567); c0 += 4;
        vout0x4567 = vout0xCDEF;
      }
      if (nc & 4) {
        vst1q_f32(c2, vout2x0123); c2 += 4;
        vout2x0123 = vout2x4567;
        vst1q_f32(c1, vout1x0123); c1 += 4;
        vout1x0123 = vout1x4567;
        vst1q_f32(c0, vout0x0123); c0 += 4;
        vout0x0123 = vout0x4567;
      }
      float32x2_t vout2x01 = vget_low_f32(vout2x0123);
      float32x2_t vout1x01 = vget_low_f32(vout1x0123);
      float32x2_t vout0x01 = vget_low_f32(vout0x0123);
      if (nc & 2) {
        vst1_f32(c2, vout2x01); c2 += 2;
        vst1_f32(c1, vout1x01); c1 += 2;
        vst1_f32(c0, vout0x01); c0 += 2;
        vout2x01 = vget_high_f32(vout2x0123);
        vout1x01 = vget_high_f32(vout1x0123);
        vout0x01 = vget_high_f32(vout0x0123);
      }
      if (nc & 1) {
        vst1_lane_f32(c2, vout2x01, 0);
        vst1_lane_f32(c1, vout1x01, 0);
        vst1_lane_f32(c0, vout0x01, 0);
      }
      nc = 0;
    }
  } while (nc != 0);
}
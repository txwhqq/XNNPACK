// Auto-generated file. Do not edit!
//   Template: src/qs8-f16-vcvt/avx2.c.in
//   Generator: tools/xngen
//
// Copyright 2023 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <immintrin.h>

#include "xnnpack/common.h"
#include "xnnpack/intrinsics-polyfill.h"
#include "xnnpack/vcvt.h"


void xnn_qs8_f16_vcvt_ukernel__avx2_u64(
    size_t batch,
    const int8_t* input,
    void* output,
    const union xnn_qs8_f16_cvt_params params[restrict XNN_MIN_ELEMENTS(1)]) XNN_OOB_READS
{
  assert(batch != 0);
  assert(batch % sizeof(int8_t) == 0);
  assert(input != NULL);
  assert(output != NULL);

  uint16_t* o = (uint16_t*) output;
  const __m256i vzero_point = _mm256_set1_epi32(params->avx.zero_point);
  const __m256 vscale = _mm256_set1_ps(params->avx.scale);
  XNN_FORCE_REALIZATION(vzero_point);
  XNN_FORCE_REALIZATION(vscale);
  for (; batch >= 64 * sizeof(int8_t); batch -= 64 * sizeof(int8_t)) {
    __m256i vx01234567 = _mm256_cvtepi8_epi32(_mm_loadl_epi64((const __m128i*) input));
    __m256i vx89ABCDEF = _mm256_cvtepi8_epi32(_mm_loadl_epi64((const __m128i*) (input + 8)));
    __m256i vxGHIJKLMN = _mm256_cvtepi8_epi32(_mm_loadl_epi64((const __m128i*) (input + 16)));
    __m256i vxOPQRSTUV = _mm256_cvtepi8_epi32(_mm_loadl_epi64((const __m128i*) (input + 24)));
    __m256i vxWXYZabcd = _mm256_cvtepi8_epi32(_mm_loadl_epi64((const __m128i*) (input + 32)));
    __m256i vxefghijkl = _mm256_cvtepi8_epi32(_mm_loadl_epi64((const __m128i*) (input + 40)));
    __m256i vxmnopqrst = _mm256_cvtepi8_epi32(_mm_loadl_epi64((const __m128i*) (input + 48)));
    __m256i vxuvwxyz01 = _mm256_cvtepi8_epi32(_mm_loadl_epi64((const __m128i*) (input + 56)));
    input += 64;

    vx01234567 = _mm256_sub_epi32(vx01234567, vzero_point);
    vx89ABCDEF = _mm256_sub_epi32(vx89ABCDEF, vzero_point);
    vxGHIJKLMN = _mm256_sub_epi32(vxGHIJKLMN, vzero_point);
    vxOPQRSTUV = _mm256_sub_epi32(vxOPQRSTUV, vzero_point);
    vxWXYZabcd = _mm256_sub_epi32(vxWXYZabcd, vzero_point);
    vxefghijkl = _mm256_sub_epi32(vxefghijkl, vzero_point);
    vxmnopqrst = _mm256_sub_epi32(vxmnopqrst, vzero_point);
    vxuvwxyz01 = _mm256_sub_epi32(vxuvwxyz01, vzero_point);

    __m256 vy01234567 = _mm256_cvtepi32_ps(vx01234567);
    __m256 vy89ABCDEF = _mm256_cvtepi32_ps(vx89ABCDEF);
    __m256 vyGHIJKLMN = _mm256_cvtepi32_ps(vxGHIJKLMN);
    __m256 vyOPQRSTUV = _mm256_cvtepi32_ps(vxOPQRSTUV);
    __m256 vyWXYZabcd = _mm256_cvtepi32_ps(vxWXYZabcd);
    __m256 vyefghijkl = _mm256_cvtepi32_ps(vxefghijkl);
    __m256 vymnopqrst = _mm256_cvtepi32_ps(vxmnopqrst);
    __m256 vyuvwxyz01 = _mm256_cvtepi32_ps(vxuvwxyz01);

    vy01234567 = _mm256_mul_ps(vy01234567, vscale);
    vy89ABCDEF = _mm256_mul_ps(vy89ABCDEF, vscale);
    vyGHIJKLMN = _mm256_mul_ps(vyGHIJKLMN, vscale);
    vyOPQRSTUV = _mm256_mul_ps(vyOPQRSTUV, vscale);
    vyWXYZabcd = _mm256_mul_ps(vyWXYZabcd, vscale);
    vyefghijkl = _mm256_mul_ps(vyefghijkl, vscale);
    vymnopqrst = _mm256_mul_ps(vymnopqrst, vscale);
    vyuvwxyz01 = _mm256_mul_ps(vyuvwxyz01, vscale);

    _mm_storeu_si128((__m128i*) o, _mm256_cvtps_ph(vy01234567, _MM_FROUND_TO_NEAREST_INT));
    _mm_storeu_si128((__m128i*) (o + 8) , _mm256_cvtps_ph(vy89ABCDEF, _MM_FROUND_TO_NEAREST_INT));
    _mm_storeu_si128((__m128i*) (o + 16) , _mm256_cvtps_ph(vyGHIJKLMN, _MM_FROUND_TO_NEAREST_INT));
    _mm_storeu_si128((__m128i*) (o + 24) , _mm256_cvtps_ph(vyOPQRSTUV, _MM_FROUND_TO_NEAREST_INT));
    _mm_storeu_si128((__m128i*) (o + 32) , _mm256_cvtps_ph(vyWXYZabcd, _MM_FROUND_TO_NEAREST_INT));
    _mm_storeu_si128((__m128i*) (o + 40) , _mm256_cvtps_ph(vyefghijkl, _MM_FROUND_TO_NEAREST_INT));
    _mm_storeu_si128((__m128i*) (o + 48) , _mm256_cvtps_ph(vymnopqrst, _MM_FROUND_TO_NEAREST_INT));
    _mm_storeu_si128((__m128i*) (o + 56) , _mm256_cvtps_ph(vyuvwxyz01, _MM_FROUND_TO_NEAREST_INT));
    o += 64;
  }
  for (; batch >= 8 * sizeof(int8_t); batch -= 8 * sizeof(int8_t)) {
    __m256i vx = _mm256_cvtepi8_epi32(_mm_loadl_epi64((const __m128i*) input));
    vx = _mm256_sub_epi32(vx, vzero_point);
    input += 8;

    __m256 vy = _mm256_cvtepi32_ps(vx);
    vy = _mm256_mul_ps(vy, vscale);

    _mm_storeu_si128((__m128i*) o, _mm256_cvtps_ph(vy, _MM_FROUND_TO_NEAREST_INT));
    o += 8;
  }
  if XNN_UNLIKELY(batch != 0) {
    assert(batch >= 1 * sizeof(int8_t));
    assert(batch <= 7 * sizeof(int8_t));

    __m256i vx = _mm256_cvtepi8_epi32(_mm_loadl_epi64((const __m128i*) input));
    vx = _mm256_sub_epi32(vx, vzero_point);

    __m256 vy = _mm256_cvtepi32_ps(vx);
    vy = _mm256_mul_ps(vy, vscale);

    __m128i vhy = _mm256_cvtps_ph(vy, _MM_FROUND_TO_NEAREST_INT);
    if (batch & (4 * sizeof(int8_t))) {
      _mm_storel_epi64((__m128i*) o, vhy);
      vhy = _mm_unpackhi_epi64(vhy, vhy);
      o += 4;
    }
    if (batch & (2 * sizeof(int8_t))) {
      _mm_storeu_si32((__m64*) o, vhy);
      vhy = _mm_srli_epi64(vhy, 32);
      o += 2;
    }
    if (batch & (1 * sizeof(int8_t))) {
      *o = (uint16_t) _mm_extract_epi16(vhy, 0);
    }
  }
}


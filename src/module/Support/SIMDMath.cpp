#include <iomanip>
#include <assert.h>
#include <utility>
#include <math.h>
#include <string.h>
#include "MathLib.h"
#include <stdlib.h>
#define ISINF(x)  ((*(uint32_t*)&(x) & 0x7FFFFFFF) == 0x7F800000)

namespace Skuld
{
		union iVec { int32_t i[4];vec v; };
		union fVec { float i[4];vec v; };
		union uVec { uint32_t i[4]; vec v; };

		static const iVec g_Infinity = { 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 };
		static const iVec g_QNaN = { 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 };
		static const fVec g_IdentityR0 = { 1.0f, 0.0f, 0.0f, 0.0f };
		static const fVec g_IdentityR1 = { 0.0f, 1.0f, 0.0f, 0.0f };
		static const fVec g_IdentityR2 = { 0.0f, 0.0f, 1.0f, 0.0f };
		static const fVec g_IdentityR3 = { 0.0f, 0.0f, 0.0f, 1.0f };
		static const iVec g_AbsMask = { 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF };
		static const fVec g_One = { 1.0f, 1.0f, 1.0f, 1.0f };
		static const fVec g_NegateX = { -1.0f, 1.0f, 1.0f, 1.0f };
		static const fVec g_NegateY = { 1.0f, -1.0f, 1.0f, 1.0f };
		static const fVec g_NegateZ = { 1.0f, 1.0f, -1.0f, 1.0f };
		static const uVec g_Mask3 = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 };
		static const uVec g_Select1110 = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 };
		static const uVec g_MaskY = { 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000 };
		static const fVec g_NegIdentityR3 = { 0.0f, 0.0f, 0.0f,-1.0f };
		static const uVec g_MaskZ = { 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000 };
		static const uVec g_MaskX = { 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000 };
		static const uVec g_Select0101 = { 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF };
		static const uVec g_FlipY = { 0,0x80000000,0,0 };
		static const fVec g_Pi = { g_PI, g_PI, g_PI, g_PI };
		static const fVec g_TwoPi = { g_PI * 2, g_PI * 2, g_PI * 2, g_PI * 2 };
		static const fVec g_HalfPi = { g_PI / 2, g_PI / 2, g_PI / 2, g_PI / 2 };
		static const uVec g_NegativeZero = { 0x80000000, 0x80000000, 0x80000000, 0x80000000 };
		static const fVec g_NoFraction = { 8388608.0f,8388608.0f,8388608.0f,8388608.0f };
		static const fVec g_ReciprocalTwoPi = { 1.0f / (g_PI * 2),1.0f / (g_PI * 2),1.0f / (g_PI * 2),1.0f / (g_PI * 2) };
		static const fVec g_SinCoefficients0 = { -0.16666667f, +0.0083333310f, -0.00019840874f, +2.7525562e-06f };
		static const fVec g_SinCoefficients1 = { -2.3889859e-08f, -0.16665852f /*Est1*/, +0.0083139502f /*Est2*/, -0.00018524670f /*Est3*/ };
		static const fVec g_CosCoefficients0 = { -0.5f, +0.041666638f, -0.0013888378f, +2.4760495e-05f };
		static const fVec g_CosCoefficients1 = { -2.6051615e-07f, -0.49992746f /*Est1*/, +0.041493919f /*Est2*/, -0.0012712436f /*Est3*/ };
		static const fVec g_NegativeOne = { -1.0f,-1.0f,-1.0f,-1.0f };

		template<int len> inline vec LoadVec(const float* f);
		template<int len> inline vec NormalizeVec(vec_f veca);
		template<int len> inline float LengthVec(vec_f veca);
		template<int len> inline void StoreVec(float* f, vec_f veca);
		template<int len> inline vec CrossVec(vec_f veca, vec_f vec2);
		template<int len> inline vec DotVec(vec_f veca, vec_f vec2);

#ifdef SIMD_NEON
#	ifdef __GNUC__
		typedef union _hvec
		{
			float32x2_t f;
			uint32x2_t u;
			uint8x8_t ba8;
			uint16x4_t sa4;
			_hvec(float32x2_t f) : f(f) {}
			_hvec(uint32x2_t u) :u(u) {}
			_hvec(uint8x8_t ba8) : ba8(ba8) {}
			_hvec(uint16x4_t sa4) : sa4(sa4) {}
		} hvec;
#		define VEC_GETU(a, i) a[i]
#		define VEC_GETF(a, i) a[i]
#	else
		typedef union _hvec
		{
			float32x2_t f;
			uint32x2_t u;
			uint8x8_t ba8;
			uint16x4_t sa4;
			_hvec(float32x2_t f) : f(f) {}
		} hvec;
#		define VEC_GETU(a, i) a.n128_u32[i]
#		define VEC_GETF(a, i) a.n128_f32[i]
#	endif

		inline bool Equal3(vec_f V1, vec_f V2)
		{
			uint32x4_t vResult = vceqq_f32(V1.f, V2.f);
			uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vec(vResult).b), vget_high_u8(vec(vResult).b));
			uint16x4x2_t Temp = vzip_u16(hvec(vTemp.val[0]).sa4, hvec(vTemp.val[1]).sa4);
			return ((vget_lane_u32(hvec(Temp.val[1]).u, 1) & 0xFFFFFFU) == 0xFFFFFFU);
		}

		inline bool IsInfinite3(vec_f V)
		{
			uint32x4_t vTempInf = vceqq_f32(vec(vandq_u32(V.u, g_AbsMask.v.u)).f, g_Infinity.v.f);
			uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vec(vTempInf).b), vget_high_u8(vec(vTempInf).b));
			uint16x4x2_t Temp = vzip_u16(hvec(vTemp.val[0]).sa4, hvec(vTemp.val[1]).sa4);
			return ((vget_lane_u32(hvec(Temp.val[1]).u, 1) & 0xFFFFFFU) != 0);
		}

		inline vec Vector(float x, float y, float z, float w)
		{
			float32x2_t V0 = vcreate_f32(((uint64_t)*(const uint32_t *)&x) | ((uint64_t)(*(const uint32_t *)&y) << 32));
			float32x2_t V1 = vcreate_f32(((uint64_t)*(const uint32_t *)&z) | ((uint64_t)(*(const uint32_t *)&w) << 32));
			return vcombine_f32(V0, V1);
		}
#endif

#if !defined(MATH_USE_SIMD)
#		define VEC_GETU(a, i) a[i]
#		define VEC_GETF(a, i) a[i]
		template<uint EX, uint EY, uint EZ, uint EW> inline vec Swizzle(vec_f v)
		{
			vec Result;
			Result.f[0] = v.f[EX];
			Result.f[1] = v.f[EY];
			Result.f[2] = v.f[EZ];
			Result.f[3] = v.f[EW];
			return Result;
		}

		template<uint PX, uint PY, uint PZ, uint PW> inline vec Permute(vec_f a, vec_f b)
		{
			const uint32_t *aPtr[2];
			aPtr[0] = (const uint32_t*)(&a);
			aPtr[1] = (const uint32_t*)(&b);

			vec Result;
			uint32_t *pWork = (uint32_t*)(&Result);

			const uint32_t i0 = PX & 3;
			const uint32_t vi0 = PX >> 2;
			pWork[0] = aPtr[vi0][i0];

			const uint32_t i1 = PY & 3;
			const uint32_t vi1 = PY >> 2;
			pWork[1] = aPtr[vi1][i1];

			const uint32_t i2 = PZ & 3;
			const uint32_t vi2 = PZ >> 2;
			pWork[2] = aPtr[vi2][i2];

			const uint32_t i3 = PW & 3;
			const uint32_t vi3 = PW >> 2;
			pWork[3] = aPtr[vi3][i3];

			return Result;
		}
#endif
#ifdef SIMD_NEON
		template<uint EX, uint EY, uint EZ, uint EW> inline vec Swizzle(vec_f v)
		{
			static const uint32_t ControlElement[4] =
			{
				0x03020100, // XM_SWIZZLE_X
				0x07060504, // XM_SWIZZLE_Y
				0x0B0A0908, // XM_SWIZZLE_Z
				0x0F0E0D0C, // XM_SWIZZLE_W
			};

			uint8x8x2_t tbl;
			tbl.val[0] = hvec(vget_low_f32(v.f)).ba8;
			tbl.val[1] = hvec(vget_high_f32(v.f)).ba8;

			uint32x2_t idx = vcreate_u32(((uint64_t)ControlElement[EX]) | (((uint64_t)ControlElement[EY]) << 32));
			const uint8x8_t rL = vtbl2_u8(vec(tbl).ba2, hvec(idx).ba8);

			idx = vcreate_u32(((uint64_t)ControlElement[EZ]) | (((uint64_t)ControlElement[EW]) << 32));
			const uint8x8_t rH = vtbl2_u8(vec(tbl).ba2, hvec(idx).ba8);

			return vcombine_f32(hvec(rL).f, hvec(rH).f);
		}

		template<uint PX, uint PY, uint PZ, uint PW> inline vec Permute(vec_f a, vec_f b)
		{
			static const uint32_t ControlElement[8] =
			{
				0x03020100, // XM_PERMUTE_0X
				0x07060504, // XM_PERMUTE_0Y
				0x0B0A0908, // XM_PERMUTE_0Z
				0x0F0E0D0C, // XM_PERMUTE_0W
				0x13121110, // XM_PERMUTE_1X
				0x17161514, // XM_PERMUTE_1Y
				0x1B1A1918, // XM_PERMUTE_1Z
				0x1F1E1D1C, // XM_PERMUTE_1W
			};

			uint8x8x4_t tbl;
			tbl.val[0] = hvec(vget_low_f32(a.f)).ba8;
			tbl.val[1] = hvec(vget_high_f32(a.f)).ba8;
			tbl.val[2] = hvec(vget_low_f32(b.f)).ba8;
			tbl.val[3] = hvec(vget_high_f32(b.f)).ba8;

			uint32x2_t idx = vcreate_u32(((uint64_t)ControlElement[PX]) | (((uint64_t)ControlElement[PY]) << 32));
			const uint8x8_t rL = vtbl4_u8(tbl, hvec(idx).ba8);

			idx = vcreate_u32(((uint64_t)ControlElement[PZ]) | (((uint64_t)ControlElement[PW]) << 32));
			const uint8x8_t rH = vtbl4_u8(tbl, hvec(idx).ba8);

			return vcombine_f32(hvec(rL).f, hvec(rH).f);
		}

		template<> inline vec Swizzle<0, 1, 2, 3>(vec_f V) { return V; }
		template<> inline vec Swizzle<0, 0, 0, 0>(vec_f V) { return vdupq_lane_f32(vget_low_f32(V.f), 0); }
		template<> inline vec Swizzle<1, 1, 1, 1>(vec_f V) { return vdupq_lane_f32(vget_low_f32(V.f), 1); }
		template<> inline vec Swizzle<2, 2, 2, 2>(vec_f V) { return vdupq_lane_f32(vget_high_f32(V.f), 0); }
		template<> inline vec Swizzle<3, 3, 3, 3>(vec_f V) { return vdupq_lane_f32(vget_high_f32(V.f), 1); }

		template<> inline vec Swizzle<1, 0, 3, 2>(vec_f V) { return vrev64q_f32(V.f); }

		template<> inline vec Swizzle<0, 1, 0, 1>(vec_f V) { float32x2_t vt = vget_low_f32(V.f); return vcombine_f32(vt, vt); }
		template<> inline vec Swizzle<2, 3, 2, 3>(vec_f V) { float32x2_t vt = vget_high_f32(V.f); return vcombine_f32(vt, vt); }
		template<> inline vec Swizzle<1, 0, 1, 0>(vec_f V) { float32x2_t vt = vrev64_f32(vget_low_f32(V.f)); return vcombine_f32(vt, vt); }
		template<> inline vec Swizzle<3, 2, 3, 2>(vec_f V) { float32x2_t vt = vrev64_f32(vget_high_f32(V.f)); return vcombine_f32(vt, vt); }

		template<> inline vec Swizzle<0, 1, 3, 2>(vec_f V) { return vcombine_f32(vget_low_f32(V.f), vrev64_f32(vget_high_f32(V.f))); }
		template<> inline vec Swizzle<1, 0, 2, 3>(vec_f V) { return vcombine_f32(vrev64_f32(vget_low_f32(V.f)), vget_high_f32(V.f)); }
		template<> inline vec Swizzle<2, 3, 1, 0>(vec_f V) { return vcombine_f32(vget_high_f32(V.f), vrev64_f32(vget_low_f32(V.f))); }
		template<> inline vec Swizzle<3, 2, 0, 1>(vec_f V) { return vcombine_f32(vrev64_f32(vget_high_f32(V.f)), vget_low_f32(V.f)); }
		template<> inline vec Swizzle<3, 2, 1, 0>(vec_f V) { return vcombine_f32(vrev64_f32(vget_high_f32(V.f)), vrev64_f32(vget_low_f32(V.f))); }

		template<> inline vec Swizzle<0, 0, 2, 2>(vec_f V) { return vtrnq_f32(V.f, V.f).val[0]; }
		template<> inline vec Swizzle<1, 1, 3, 3>(vec_f V) { return vtrnq_f32(V.f, V.f).val[1]; }

		template<> inline vec Swizzle<0, 0, 1, 1>(vec_f V) { return vzipq_f32(V.f, V.f).val[0]; }
		template<> inline vec Swizzle<2, 2, 3, 3>(vec_f V) { return vzipq_f32(V.f, V.f).val[1]; }

		template<> inline vec Swizzle<0, 2, 0, 2>(vec_f V) { return vuzpq_f32(V.f, V.f).val[0]; }
		template<> inline vec Swizzle<1, 3, 1, 3>(vec_f V) { return vuzpq_f32(V.f, V.f).val[1]; }

		template<> inline vec Swizzle<1, 2, 3, 0>(vec_f V) { return vextq_f32(V.f, V.f, 1); }
		template<> inline vec Swizzle<2, 3, 0, 1>(vec_f V) { return vextq_f32(V.f, V.f, 2); }
		template<> inline vec Swizzle<3, 0, 1, 2>(vec_f V) { return vextq_f32(V.f, V.f, 3); }


		template<> inline vec Permute<0, 1, 2, 3>(vec_f V1, vec_f) { return V1; }
		template<> inline vec Permute<4, 5, 6, 7>(vec_f, vec_f V2) { return V2; }

		template<> inline vec Permute<0, 1, 4, 5>(vec_f V1, vec_f V2) { return vcombine_f32(vget_low_f32(V1.f), vget_low_f32(V2.f)); }
		template<> inline vec Permute<1, 0, 4, 5>(vec_f V1, vec_f V2) { return vcombine_f32(vrev64_f32(vget_low_f32(V1.f)), vget_low_f32(V2.f)); }
		template<> inline vec Permute<0, 1, 5, 4>(vec_f V1, vec_f V2) { return vcombine_f32(vget_low_f32(V1.f), vrev64_f32(vget_low_f32(V2.f))); }
		template<> inline vec Permute<1, 0, 5, 4>(vec_f V1, vec_f V2) { return vcombine_f32(vrev64_f32(vget_low_f32(V1.f)), vrev64_f32(vget_low_f32(V2.f))); }

		template<> inline vec Permute<2, 3, 6, 7>(vec_f V1, vec_f V2) { return vcombine_f32(vget_high_f32(V1.f), vget_high_f32(V2.f)); }
		template<> inline vec Permute<3, 2, 6, 7>(vec_f V1, vec_f V2) { return vcombine_f32(vrev64_f32(vget_high_f32(V1.f)), vget_high_f32(V2.f)); }
		template<> inline vec Permute<2, 3, 7, 6>(vec_f V1, vec_f V2) { return vcombine_f32(vget_high_f32(V1.f), vrev64_f32(vget_high_f32(V2.f))); }
		template<> inline vec Permute<3, 2, 7, 6>(vec_f V1, vec_f V2) { return vcombine_f32(vrev64_f32(vget_high_f32(V1.f)), vrev64_f32(vget_high_f32(V2.f))); }

		template<> inline vec Permute<0, 1, 6, 7>(vec_f V1, vec_f V2) { return vcombine_f32(vget_low_f32(V1.f), vget_high_f32(V2.f)); }
		template<> inline vec Permute<1, 0, 6, 7>(vec_f V1, vec_f V2) { return vcombine_f32(vrev64_f32(vget_low_f32(V1.f)), vget_high_f32(V2.f)); }
		template<> inline vec Permute<0, 1, 7, 6>(vec_f V1, vec_f V2) { return vcombine_f32(vget_low_f32(V1.f), vrev64_f32(vget_high_f32(V2.f))); }
		template<> inline vec Permute<1, 0, 7, 6>(vec_f V1, vec_f V2) { return vcombine_f32(vrev64_f32(vget_low_f32(V1.f)), vrev64_f32(vget_high_f32(V2.f))); }

		template<> inline vec Permute<3, 2, 4, 5>(vec_f V1, vec_f V2) { return vcombine_f32(vrev64_f32(vget_high_f32(V1.f)), vget_low_f32(V2.f)); }
		template<> inline vec Permute<2, 3, 5, 4>(vec_f V1, vec_f V2) { return vcombine_f32(vget_high_f32(V1.f), vrev64_f32(vget_low_f32(V2.f))); }
		template<> inline vec Permute<3, 2, 5, 4>(vec_f V1, vec_f V2) { return vcombine_f32(vrev64_f32(vget_high_f32(V1.f)), vrev64_f32(vget_low_f32(V2.f))); }

		template<> inline vec Permute<0, 4, 2, 6>(vec_f V1, vec_f V2) { return vtrnq_f32(V1.f, V2.f).val[0]; }
		template<> inline vec Permute<1, 5, 3, 7>(vec_f V1, vec_f V2) { return vtrnq_f32(V1.f, V2.f).val[1]; }

		template<> inline vec Permute<0, 4, 1, 5>(vec_f V1, vec_f V2) { return vzipq_f32(V1.f, V2.f).val[0]; }
		template<> inline vec Permute<2, 6, 3, 7>(vec_f V1, vec_f V2) { return vzipq_f32(V1.f, V2.f).val[1]; }

		template<> inline vec Permute<0, 2, 4, 6>(vec_f V1, vec_f V2) { return vuzpq_f32(V1.f, V2.f).val[0]; }
		template<> inline vec Permute<1, 3, 5, 7>(vec_f V1, vec_f V2) { return vuzpq_f32(V1.f, V2.f).val[1]; }

		template<> inline vec Permute<1, 2, 3, 4>(vec_f V1, vec_f V2) { return vextq_f32(V1.f, V2.f, 1); }
		template<> inline vec Permute<2, 3, 4, 5>(vec_f V1, vec_f V2) { return vextq_f32(V1.f, V2.f, 2); }
		template<> inline vec Permute<3, 4, 5, 6>(vec_f V1, vec_f V2) { return vextq_f32(V1.f, V2.f, 3); }

#endif

		template<> inline vec LoadVec<2>(const float* f)
		{
#ifndef MATH_USE_SIMD
			vec ret;
			ret.f[0] = f[0];
			ret.f[1] = f[1];
			ret.f[2] = 0.f;
			ret.f[3] = 0.f;
			return ret;
#else
#	if defined(SIMD_SSE2)
			__m128 x = _mm_load_ss(&f[0]);
			__m128 y = _mm_load_ss(&f[1]);
			return _mm_unpacklo_ps(x, y);
#	else
			float32x2_t x = vld1_f32(f);
			float32x2_t zero = vdup_n_f32(0);
			return vcombine_f32(x, zero);
#	endif
#endif
		}

		template<> inline vec LoadVec<3>(const float* f)
		{
#ifndef MATH_USE_SIMD
			vec ret;
			ret.f[0] = f[0];
			ret.f[1] = f[1];
			ret.f[2] = f[2];
			ret.f[3] = 0.f;
			return ret;
#else
#	if defined(SIMD_SSE2)
			__m128 x = _mm_load_ss(&f[0]);
			__m128 y = _mm_load_ss(&f[1]);
			__m128 z = _mm_load_ss(&f[2]);
			__m128 xy = _mm_unpacklo_ps(x, y);
			return _mm_movelh_ps(xy, z);
#	else
			float32x2_t x = vld1_f32(f);
			float32x2_t zero = vdup_n_f32(0);
			float32x2_t y = vld1_lane_f32(f + 2, zero, 0);
			return vcombine_f32(x, y);
#	endif
#endif
		}

		template<> inline vec LoadVec<4>(const float* f)
		{
#ifndef MATH_USE_SIMD
			vec ret;
			ret.f[0] = f[0];
			ret.f[1] = f[1];
			ret.f[2] = f[2];
			ret.f[3] = f[3];
			return ret;
#else
#	if defined(SIMD_SSE2)
			return _mm_loadu_ps(f);
#	else
			return vld1q_f32(f);
#	endif
#endif
		}
		
		template<> inline void StoreVec<2>(float* f, vec_f v)
		{
#ifndef MATH_USE_SIMD
			f[0] = v.f[0];
			f[1] = v.f[1];
#else
#	if defined(SIMD_SSE2)
			__m128 T = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
			_mm_store_ss(&f[0], v);
			_mm_store_ss(&f[1], T);
#	else
			float32x2_t VL = vget_low_f32(v.f);
			vst1_f32(f, VL);
#	endif
#endif
		}

		template<> inline void StoreVec<3>(float* f, vec_f v)
		{
#ifndef MATH_USE_SIMD
			f[0] = v.f[0];
			f[1] = v.f[1];
			f[2] = v.f[2];
#else
#	if defined(SIMD_SSE2)
			__m128 T1 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
			__m128 T2 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2));
			_mm_store_ss(&f[0], v);
			_mm_store_ss(&f[1], T1);
			_mm_store_ss(&f[2], T2);
#	else
			float32x2_t VL = vget_low_f32(v.f);
			vst1_f32(f, VL);
			vst1q_lane_f32(f + 2, v.f, 2);
#	endif
#endif
		}

		template<> inline void StoreVec<4>(float* f, vec_f v)
		{
#ifndef MATH_USE_SIMD
			f[0] = v.f[0];
			f[1] = v.f[1];
			f[2] = v.f[2];
			f[3] = v.f[3];
#else
#	if defined(SIMD_SSE2)
			_mm_storeu_ps(f, v);
#	else
			vst1q_f32(f, v.f);
#	endif
#endif
		}

		template<> inline vec DotVec<4>(vec_f a, vec_f b)
		{
#ifndef MATH_USE_SIMD
			vec Result;
			Result.f[0] = Result.f[1] = Result.f[2] = Result.f[3] =
				a.f[0] * b.f[0] +
				a.f[1] * b.f[1] +
				a.f[2] * b.f[2] +
				a.f[3] * b.f[3];
			return Result;
#else
#	if defined(SIMD_SSE2)
			__m128 vTemp2 = b;
			__m128 vTemp = _mm_mul_ps(a, vTemp2);
			vTemp2 = _mm_shuffle_ps(vTemp2, vTemp, _MM_SHUFFLE(1, 0, 0, 0)); // Copy X to the Z position and Y to the W position
			vTemp2 = _mm_add_ps(vTemp2, vTemp);          // Add Z = X+Z; W = Y+W;
			vTemp = _mm_shuffle_ps(vTemp, vTemp2, _MM_SHUFFLE(0, 3, 0, 0));  // Copy W to the Z position
			vTemp = _mm_add_ps(vTemp, vTemp2);           // Add Z and W together
			return _mm_shuffle_ps(vTemp, vTemp, _MM_SHUFFLE(2, 2, 2, 2));    // Splat Z and return
#	else
			float32x4_t vTemp = vmulq_f32(a.f, b.f);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vadd_f32(v1, v2);
			v1 = vpadd_f32(v1, v1);
			return vcombine_f32(v1, v1);
#	endif
#endif
		}

		template<> inline vec DotVec<3>(vec_f a, vec_f b)
		{
#ifndef MATH_USE_SIMD
			float fValue =
				a.f[0] * b.f[0] +
				a.f[1] * b.f[1] +
				a.f[2] * b.f[2];
			vec vResult;
			vResult.f[0] =
				vResult.f[1] =
				vResult.f[2] =
				vResult.f[3] = fValue;
			return vResult;
#else
#	if defined(SIMD_SSE2)
			__m128 vDot = _mm_mul_ps(a, b);
			__m128 vTemp = _mm_shuffle_ps(vDot, vDot, _MM_SHUFFLE(2, 1, 2, 1));
			vDot = _mm_add_ss(vDot, vTemp);
			vTemp = _mm_shuffle_ps(vTemp, vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			vDot = _mm_add_ss(vDot, vTemp);
			return _mm_shuffle_ps(vDot, vDot, _MM_SHUFFLE(0, 0, 0, 0));
#	else
			float32x4_t vTemp = vmulq_f32(a.f, b.f);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vpadd_f32(v1, v1);
			v2 = vdup_lane_f32(v2, 0);
			v1 = vadd_f32(v1, v2);
			return vcombine_f32(v1, v1);
#	endif
#endif
		}
		
		template<> inline vec DotVec<2>(vec_f a, vec_f b)
		{
#ifndef MATH_USE_SIMD
			vec Result;
			Result.f[0] =
				Result.f[1] =
				Result.f[2] =
				Result.f[3] = a.f[0] * b.f[0] + a.f[1] * b.f[1];
			return Result;
#else
#	if defined(SIMD_SSE2)
			__m128 vLengthSq = _mm_mul_ps(a, b);
			__m128 vTemp = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			return vLengthSq;
#	else
			float32x2_t vTemp = vmul_f32(vget_low_f32(a.f), vget_low_f32(b.f));
			vTemp = vpadd_f32(vTemp, vTemp);
			return vcombine_f32(vTemp, vTemp);
#	endif
#endif
		}

		template<> inline float LengthVec<4>(vec_f v)
		{
#ifndef MATH_USE_SIMD
			vec Result;

			Result = DotVec<4>(v, v);
			return sqrt(Result.f[0]);
#else
#	if defined(SIMD_SSE2)
			__m128 vLengthSq = _mm_mul_ps(v, v);
			__m128 vTemp = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			vLengthSq = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
			vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			vLengthSq = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return get(vLengthSq, 0);
#	else
			// Dot4
			float32x4_t vTemp = vmulq_f32(v.f, v.f);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vadd_f32(v1, v2);
			v1 = vpadd_f32(v1, v1);
			const float32x2_t zero = vdup_n_f32(0);
			uint32x2_t VEqualsZero = vceq_f32(v1, zero);
			// Sqrt
			float32x2_t S0 = vrsqrte_f32(v1);
			float32x2_t P0 = vmul_f32(v1, S0);
			float32x2_t R0 = vrsqrts_f32(P0, S0);
			float32x2_t S1 = vmul_f32(S0, R0);
			float32x2_t P1 = vmul_f32(v1, S1);
			float32x2_t R1 = vrsqrts_f32(P1, S1);
			float32x2_t Result = vmul_f32(S1, R1);
			Result = vmul_f32(v1, Result);
			Result = vbsl_f32(VEqualsZero, zero, Result);
			float32x4_t ret = vcombine_f32(Result, Result);
			return *reinterpret_cast<float*>(&ret);
#	endif
#endif
		}

		template<> inline float LengthVec<3>(vec_f v)
		{
#ifndef MATH_USE_SIMD
			vec Result;

			Result = DotVec<3>(v, v);
			return sqrt(Result.f[0]);
#else
#	if defined(SIMD_SSE2)
			__m128 vLengthSq = _mm_mul_ps(v, v);
			__m128 vTemp = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 2, 1, 2));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vTemp = _mm_shuffle_ps(vTemp, vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return get(vLengthSq, 0);
#	else
			// Dot3
			float32x4_t vTemp = vmulq_f32(v.f, v.f);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vpadd_f32(v1, v1);
			v2 = vdup_lane_f32(v2, 0);
			v1 = vadd_f32(v1, v2);
			const float32x2_t zero = vdup_n_f32(0);
			uint32x2_t VEqualsZero = vceq_f32(v1, zero);
			// Sqrt
			float32x2_t S0 = vrsqrte_f32(v1);
			float32x2_t P0 = vmul_f32(v1, S0);
			float32x2_t R0 = vrsqrts_f32(P0, S0);
			float32x2_t S1 = vmul_f32(S0, R0);
			float32x2_t P1 = vmul_f32(v1, S1);
			float32x2_t R1 = vrsqrts_f32(P1, S1);
			float32x2_t Result = vmul_f32(S1, R1);
			Result = vmul_f32(v1, Result);
			Result = vbsl_f32(VEqualsZero, zero, Result);
			float32x4_t ret = vcombine_f32(Result, Result);
			return *reinterpret_cast<float*>(&ret);
#	endif
#endif
		}

		template<> inline float LengthVec<2>(vec_f v)
		{
#ifndef MATH_USE_SIMD
			vec Result;

			Result = DotVec<2>(v, v);
			return sqrt(Result.f[0]);
#else
#	if defined(SIMD_SSE2)
			__m128 vLengthSq = _mm_mul_ps(v, v);
			__m128 vTemp = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			vLengthSq = _mm_sqrt_ps(vLengthSq);
			return get(vLengthSq, 0);
#	else
			float32x2_t VL = vget_low_f32(v.f);
			// Dot2
			float32x2_t vTemp = vmul_f32(VL, VL);
			vTemp = vpadd_f32(vTemp, vTemp);
			const float32x2_t zero = vdup_n_f32(0);
			uint32x2_t VEqualsZero = vceq_f32(vTemp, zero);
			// Sqrt
			float32x2_t S0 = vrsqrte_f32(vTemp);
			float32x2_t P0 = vmul_f32(vTemp, S0);
			float32x2_t R0 = vrsqrts_f32(P0, S0);
			float32x2_t S1 = vmul_f32(S0, R0);
			float32x2_t P1 = vmul_f32(vTemp, S1);
			float32x2_t R1 = vrsqrts_f32(P1, S1);
			float32x2_t Result = vmul_f32(S1, R1);
			Result = vmul_f32(vTemp, Result);
			Result = vbsl_f32(VEqualsZero, zero, Result);
			float32x4_t ret = vcombine_f32(Result, Result);
			return *reinterpret_cast<float*>(&ret);
#	endif
#endif
		}
		
		inline vec SqrtVec(vec_f v)
		{
#ifndef MATH_USE_SIMD
			vec r;
			r.f[0] = sqrt(v.f[0]);
			r.f[1] = sqrt(v.f[1]);
			r.f[2] = sqrt(v.f[2]);
			r.f[3] = sqrt(v.f[3]);
			return r;
#else
#	if defined(SIMD_SSE2)
			return _mm_sqrt_ps(v);
#	else
			// 3 iterations of Newton-Raphson refinment of sqrt
			float32x4_t S0 = vrsqrteq_f32(v.f);
			float32x4_t P0 = vmulq_f32(v.f, S0);
			float32x4_t R0 = vrsqrtsq_f32(P0, S0);
			float32x4_t S1 = vmulq_f32(S0, R0);
			float32x4_t P1 = vmulq_f32(v.f, S1);
			float32x4_t R1 = vrsqrtsq_f32(P1, S1);
			float32x4_t S2 = vmulq_f32(S1, R1);
			float32x4_t P2 = vmulq_f32(v.f, S2);
			float32x4_t R2 = vrsqrtsq_f32(P2, S2);
			float32x4_t S3 = vmulq_f32(S2, R2);

			vec VEqualsInfinity = vceqq_u32(v.u, g_Infinity.v.u);
			vec VEqualsZero = vceqq_f32(v.f, vdupq_n_f32(0));
			vec Result = vmulq_f32(v.f, S3);
			vec Select = vceqq_u32(VEqualsInfinity.u, VEqualsZero.u);

			return vbslq_f32(Select.u, Result.f, v.f);
#	endif
#endif
		}
		
		inline vec MultiplyVec(vec_f v, vec_f v2)
		{
#ifndef MATH_USE_SIMD
			vec Result;
			Result.f[0] = v.f[0] * v2.f[0];
			Result.f[1] = v.f[1] * v2.f[1];
			Result.f[2] = v.f[2] * v2.f[2];
			Result.f[3] = v.f[3] * v2.f[3];
			return Result;
#else
#	if defined(SIMD_SSE2)
			return _mm_mul_ps(v, v2);
#	else
			return vmulq_f32(v.f, v2.f);
#	endif
#endif
		}
		
		inline vec AddVec(vec_f v, vec_f v2)
		{
#ifndef MATH_USE_SIMD
			vec Result;
			Result.f[0] = v.f[0] + v2.f[0];
			Result.f[1] = v.f[1] + v2.f[1];
			Result.f[2] = v.f[2] + v2.f[2];
			Result.f[3] = v.f[3] + v2.f[3];
			return Result;
#else
#	if defined(SIMD_SSE2)
			return _mm_add_ps(v, v2);
#	else
			return vaddq_f32(v.f, v2.f);
#	endif
#endif
		}
		
		inline vec SubVec(vec_f v, vec_f v2)
		{
#ifndef MATH_USE_SIMD
			vec Result;
			Result.f[0] = v.f[0] - v2.f[0];
			Result.f[1] = v.f[1] - v2.f[1];
			Result.f[2] = v.f[2] - v2.f[2];
			Result.f[3] = v.f[3] - v2.f[3];
			return Result;
#else
#	if defined(SIMD_SSE2)
			return _mm_sub_ps(v, v2);
#	else
			return vsubq_f32(v.f, v2.f);
#	endif
#endif
		}
		
		inline vec InverseSqrtVec(vec_f v)
		{
#ifndef MATH_USE_SIMD
			vec r;
			r.f[0] = inversesqrt(v.f[0]);
			r.f[1] = inversesqrt(v.f[1]);
			r.f[2] = inversesqrt(v.f[2]);
			r.f[3] = inversesqrt(v.f[3]);
			return r;
#else
#	if defined(SIMD_SSE2)
			return _mm_rsqrt_ps(v);
#	else
			float32x4_t S0 = vrsqrteq_f32(v.f);

			float32x4_t P0 = vmulq_f32(v.f, S0);
			float32x4_t R0 = vrsqrtsq_f32(P0, S0);

			float32x4_t S1 = vmulq_f32(S0, R0);
			float32x4_t P1 = vmulq_f32(v.f, S1);
			float32x4_t R1 = vrsqrtsq_f32(P1, S1);

			return vmulq_f32(S1, R1);
#	endif
#endif
		}
		
		template<> inline vec CrossVec<3>(vec_f a, vec_f b)
		{
#ifndef MATH_USE_SIMD
			vec vResult;
			vResult.f[0] = (a.f[1] * b.f[2]) - (a.f[2] * b.f[1]);
			vResult.f[1] = (a.f[2] * b.f[0]) - (a.f[0] * b.f[2]);
			vResult.f[2] = (a.f[0] * b.f[1]) - (a.f[1] * b.f[0]);
			vResult.f[3] = 0.0f;
			return vResult;
#else
#	if defined(SIMD_SSE2)
			__m128 vTemp1 = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));
			__m128 vTemp2 = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2));
			__m128 vResult = _mm_mul_ps(vTemp1, vTemp2);
			vTemp1 = _mm_shuffle_ps(vTemp1, vTemp1, _MM_SHUFFLE(3, 0, 2, 1));
			vTemp2 = _mm_shuffle_ps(vTemp2, vTemp2, _MM_SHUFFLE(3, 1, 0, 2));
			vTemp1 = _mm_mul_ps(vTemp1, vTemp2);
			vResult = _mm_sub_ps(vResult, vTemp1);
			return _mm_and_ps(vResult, g_Mask3.v);
#	else
			float32x2_t v1xy = vget_low_f32(a.f);
			float32x2_t v2xy = vget_low_f32(b.f);

			float32x2_t v1yx = vrev64_f32(v1xy);
			float32x2_t v2yx = vrev64_f32(v2xy);

			float32x2_t v1zz = vdup_lane_f32(vget_high_f32(a.f), 0);
			float32x2_t v2zz = vdup_lane_f32(vget_high_f32(b.f), 0);

			vec vResult = vmulq_f32(vcombine_f32(v1yx, v1xy), vcombine_f32(v2zz, v2yx));
			vResult = vmlsq_f32(vResult.f, vcombine_f32(v1zz, v1yx), vcombine_f32(v2yx, v2xy));
			vResult = veorq_u32(vResult.u, g_FlipY.v.u);
			return vandq_u32(vResult.u, g_Mask3.v.u);
#	endif
#endif
		}

		template<> inline vec CrossVec<2>(vec_f veca, vec_f vec2)
		{
#ifndef MATH_USE_SIMD
			float fCross = (veca.f[0] * vec2.f[1]) - (veca.f[1] * vec2.f[0]);
			vec vResult;
			vResult.f[0] =
				vResult.f[1] =
				vResult.f[2] =
				vResult.f[3] = fCross;
			return vResult;
#else
#	if defined(SIMD_SSE2)
			__m128 vResult = _mm_shuffle_ps(vec2, vec2, _MM_SHUFFLE(0, 1, 0, 1));
			vResult = _mm_mul_ps(vResult, veca);
			__m128 vTemp = _mm_shuffle_ps(vResult, vResult, _MM_SHUFFLE(1, 1, 1, 1));
			vResult = _mm_sub_ss(vResult, vTemp);
			vResult = _mm_shuffle_ps(vResult, vResult, _MM_SHUFFLE(0, 0, 0, 0));
			return vResult;
#	else
			static const fVec Negate = { 1.f, -1.f, 0, 0 };

			float32x2_t vTemp = vmul_f32(vget_low_f32(veca.f), vrev64_f32(vget_low_f32(vec2.f)));
			vTemp = vmul_f32(vTemp, vget_low_f32(Negate.v.f));
			vTemp = vpadd_f32(vTemp, vTemp);
			return vcombine_f32(vTemp, vTemp);
#	endif
#endif
		}

		inline vec CrossVec4(vec_f veca, vec_f vec2, vec_f vec3)
		{
#ifndef MATH_USE_SIMD
			vec Result;
			Result.f[0] = (((vec2.f[2] * vec3.f[3]) - (vec2.f[3] * vec3.f[2]))*veca.f[1]) - (((vec2.f[1] * vec3.f[3]) - (vec2.f[3] * vec3.f[1]))*veca.f[2]) + (((vec2.f[1] * vec3.f[2]) - (vec2.f[2] * vec3.f[1]))*veca.f[3]);
			Result.f[1] = (((vec2.f[3] * vec3.f[2]) - (vec2.f[2] * vec3.f[3]))*veca.f[0]) - (((vec2.f[3] * vec3.f[0]) - (vec2.f[0] * vec3.f[3]))*veca.f[2]) + (((vec2.f[2] * vec3.f[0]) - (vec2.f[0] * vec3.f[2]))*veca.f[3]);
			Result.f[2] = (((vec2.f[1] * vec3.f[3]) - (vec2.f[3] * vec3.f[1]))*veca.f[0]) - (((vec2.f[0] * vec3.f[3]) - (vec2.f[3] * vec3.f[0]))*veca.f[1]) + (((vec2.f[0] * vec3.f[1]) - (vec2.f[1] * vec3.f[0]))*veca.f[3]);
			Result.f[3] = (((vec2.f[2] * vec3.f[1]) - (vec2.f[1] * vec3.f[2]))*veca.f[0]) - (((vec2.f[2] * vec3.f[0]) - (vec2.f[0] * vec3.f[2]))*veca.f[1]) + (((vec2.f[1] * vec3.f[0]) - (vec2.f[0] * vec3.f[1]))*veca.f[2]);
			return Result;
#else
#	if defined(SIMD_SSE2)
			// V2zwyz * V3wzwy
			__m128 vResult = _mm_shuffle_ps(vec2, vec2, _MM_SHUFFLE(2, 1, 3, 2));
			__m128 vTemp3 = _mm_shuffle_ps(vec3, vec3, _MM_SHUFFLE(1, 3, 2, 3));
			vResult = _mm_mul_ps(vResult, vTemp3);
			// - V2wzwy * V3zwyz
			__m128 vTemp2 = _mm_shuffle_ps(vec2, vec2, _MM_SHUFFLE(1, 3, 2, 3));
			vTemp3 = _mm_shuffle_ps(vTemp3, vTemp3, _MM_SHUFFLE(1, 3, 0, 1));
			vTemp2 = _mm_mul_ps(vTemp2, vTemp3);
			vResult = _mm_sub_ps(vResult, vTemp2);
			// term1 * V1yxxx
			__m128 vTemp1 = _mm_shuffle_ps(veca, veca, _MM_SHUFFLE(0, 0, 0, 1));
			vResult = _mm_mul_ps(vResult, vTemp1);
			vTemp2 = _mm_shuffle_ps(vec2, vec2, _MM_SHUFFLE(2, 0, 3, 1));
			vTemp3 = _mm_shuffle_ps(vec3, vec3, _MM_SHUFFLE(0, 3, 0, 3));
			vTemp3 = _mm_mul_ps(vTemp3, vTemp2);
			vTemp2 = _mm_shuffle_ps(vTemp2, vTemp2,  _MM_SHUFFLE(2, 1, 2, 1));
			vTemp1 = _mm_shuffle_ps(vec3, vec3, _MM_SHUFFLE(2, 0, 3, 1));
			vTemp2 = _mm_mul_ps(vTemp2, vTemp1);
			vTemp3 = _mm_sub_ps(vTemp3, vTemp2);
			vTemp1 = _mm_shuffle_ps(veca, veca, _MM_SHUFFLE(1, 1, 2, 2));
			vTemp1 = _mm_mul_ps(vTemp1, vTemp3);
			vResult = _mm_sub_ps(vResult, vTemp1);
			vTemp2 = _mm_shuffle_ps(vec2, vec2, _MM_SHUFFLE(1, 0, 2, 1));
			vTemp3 = _mm_shuffle_ps(vec3, vec2, _MM_SHUFFLE(0, 1, 0, 2));
			vTemp3 = _mm_mul_ps(vTemp3, vTemp2);
			vTemp2 = _mm_shuffle_ps(vTemp2, vTemp2, _MM_SHUFFLE(2, 0, 2, 1));
			vTemp1 = _mm_shuffle_ps(vec3, vec3, _MM_SHUFFLE(1, 0, 2, 1));
			vTemp1 = _mm_mul_ps(vTemp1, vTemp2);
			vTemp3 = _mm_sub_ps(vTemp3, vTemp1);
			vTemp1 = _mm_shuffle_ps(veca, veca, _MM_SHUFFLE(2, 3, 3, 3));
			vTemp3 = _mm_mul_ps(vTemp3, vTemp1);
			vResult = _mm_add_ps(vResult, vTemp3);
			return vResult;
#	else
			const float32x2_t select = vget_low_f32(g_MaskX.v.f);
			const float32x2_t v2xy = vget_low_f32(vec2.f);
			const float32x2_t v2zw = vget_high_f32(vec2.f);
			const float32x2_t v2yx = vrev64_f32(v2xy);
			const float32x2_t v2wz = vrev64_f32(v2zw);
			const float32x2_t v2yz = vbsl_f32(hvec(select).u, v2yx, v2wz);

			const float32x2_t v3zw = vget_high_f32(vec3.f);
			const float32x2_t v3wz = vrev64_f32(v3zw);
			const float32x2_t v3xy = vget_low_f32(vec3.f);
			const float32x2_t v3wy = vbsl_f32(hvec(select).u, v3wz, v3xy);

			float32x4_t vTemp1 = vcombine_f32(v2zw, v2yz);
			float32x4_t vTemp2 = vcombine_f32(v3wz, v3wy);
			vec vResult = vmulq_f32(vTemp1, vTemp2);
			const float32x2_t v2wy = vbsl_f32(hvec(select).u, v2wz, v2xy);
			const float32x2_t v3yx = vrev64_f32(v3xy);
			const float32x2_t v3yz = vbsl_f32(hvec(select).u, v3yx, v3wz);
			vTemp1 = vcombine_f32(v2wz, v2wy);
			vTemp2 = vcombine_f32(v3zw, v3yz);
			vResult = vmlsq_f32(vResult.f, vTemp1, vTemp2);
			const float32x2_t v1xy = vget_low_f32(veca.f);
			const float32x2_t v1yx = vrev64_f32(v1xy);
			vTemp1 = vcombine_f32(v1yx, vdup_lane_f32(v1yx, 1));
			vResult = vmulq_f32(vResult.f, vTemp1);
			const float32x2_t v2yw = vrev64_f32(v2wy);
			const float32x2_t v2xz = vbsl_f32(hvec(select).u, v2xy, v2wz);
			const float32x2_t v3wx = vbsl_f32(hvec(select).u, v3wz, v3yx);
			vTemp1 = vcombine_f32(v2yw, v2xz);
			vTemp2 = vcombine_f32(v3wx, v3wx);
			float32x4_t vTerm = vmulq_f32(vTemp1, vTemp2);
			const float32x2_t v2wx = vbsl_f32(hvec(select).u, v2wz, v2yx);
			const float32x2_t v3yw = vrev64_f32(v3wy);
			const float32x2_t v3xz = vbsl_f32(hvec(select).u, v3xy, v3wz);
			vTemp1 = vcombine_f32(v2wx, v2wx);
			vTemp2 = vcombine_f32(v3yw, v3xz);
			vTerm = vmlsq_f32(vTerm, vTemp1, vTemp2);
			const float32x2_t v1zw = vget_high_f32(veca.f);
			vTemp1 = vcombine_f32(vdup_lane_f32(v1zw, 0), vdup_lane_f32(v1yx, 0));
			vResult = vmlsq_f32(vResult.f, vTerm, vTemp1);
			const float32x2_t v3zx = vrev64_f32(v3xz);
			vTemp1 = vcombine_f32(v2yz, v2xy);
			vTemp2 = vcombine_f32(v3zx, v3yx);
			vTerm = vmulq_f32(vTemp1, vTemp2);
			const float32x2_t v2zx = vrev64_f32(v2xz);
			vTemp1 = vcombine_f32(v2zx, v2yx);
			vTemp2 = vcombine_f32(v3yz, v3xy);
			vTerm = vmlsq_f32(vTerm, vTemp1, vTemp2);
			const float32x2_t v1wz = vrev64_f32(v1zw);
			vTemp1 = vcombine_f32(vdup_lane_f32(v1wz, 0), v1wz);
			return vmlaq_f32(vResult.f, vTerm, vTemp1);
#	endif
#endif
		}
		
		template<> inline vec NormalizeVec<2>(vec_f v)
		{
#ifndef MATH_USE_SIMD
			vec vResult = DotVec<2>(v, v);

			float fLength = sqrt(vResult.f[0]);

			if (fLength > 0) {
				fLength = 1.0f / fLength;
			}

			vResult.f[0] = v.f[0] * fLength;
			vResult.f[1] = v.f[1] * fLength;
			vResult.f[2] = v.f[2] * fLength;
			vResult.f[3] = v.f[3] * fLength;
			return vResult;
#else
#	if defined(SIMD_SSE2)
			__m128 vLengthSq = _mm_mul_ps(v, v);
			__m128 vTemp = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 vResult = _mm_sqrt_ps(vLengthSq);
			__m128 vZeroMask = _mm_setzero_ps();
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			vLengthSq = _mm_cmpneq_ps(vLengthSq, g_Infinity.v);
			vResult = _mm_div_ps(v, vResult);
			vResult = _mm_and_ps(vResult, vZeroMask);
			__m128 vTemp1 = _mm_andnot_ps(vLengthSq, g_QNaN.v);
			__m128 vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#	else
			float32x2_t VL = vget_low_f32(v.f);
			// Dot2
			float32x2_t vTemp = vmul_f32(VL, VL);
			vTemp = vpadd_f32(vTemp, vTemp);
			uint32x2_t VEqualsZero = vceq_f32(vTemp, vdup_n_f32(0));
			uint32x2_t VEqualsInf = vceq_f32(vTemp, vget_low_f32(g_Infinity.v.f));
			// Reciprocal sqrt (2 iterations of Newton-Raphson)
			float32x2_t S0 = vrsqrte_f32(vTemp);
			float32x2_t P0 = vmul_f32(vTemp, S0);
			float32x2_t R0 = vrsqrts_f32(P0, S0);
			float32x2_t S1 = vmul_f32(S0, R0);
			float32x2_t P1 = vmul_f32(vTemp, S1);
			float32x2_t R1 = vrsqrts_f32(P1, S1);
			vTemp = vmul_f32(S1, R1);
			// Normalize
			float32x2_t Result = vmul_f32(VL, vTemp);
			Result = vbsl_f32(VEqualsZero, vdup_n_f32(0), Result);
			Result = vbsl_f32(VEqualsInf, vget_low_f32(g_QNaN.v.f), Result);
			return vcombine_f32(Result, Result);
#	endif
#endif
		}

		template<> inline vec NormalizeVec<3>(vec_f v)
		{
#ifndef MATH_USE_SIMD
			float fLength;
			vec vResult;

			fLength = LengthVec<3>(v);
			if (fLength > 0) {
				fLength = 1.0f / fLength;
			}

			vResult.f[0] = v.f[0] * fLength;
			vResult.f[1] = v.f[1] * fLength;
			vResult.f[2] = v.f[2] * fLength;
			vResult.f[3] = v.f[3] * fLength;
			return vResult;
#else
#	if defined(SIMD_SSE2)
			__m128 vLengthSq = _mm_mul_ps(v, v);
			__m128 vTemp = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(2, 1, 2, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vTemp = _mm_shuffle_ps(vTemp, vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 vResult = _mm_sqrt_ps(vLengthSq);
			__m128 vZeroMask = _mm_setzero_ps();
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			vLengthSq = _mm_cmpneq_ps(vLengthSq, g_Infinity.v);
			vResult = _mm_div_ps(v, vResult);
			vResult = _mm_and_ps(vResult, vZeroMask);
			__m128 vTemp1 = _mm_andnot_ps(vLengthSq, g_QNaN.v);
			__m128 vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#	else
			float32x4_t vTemp = vmulq_f32(v.f, v.f);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vpadd_f32(v1, v1);
			v2 = vdup_lane_f32(v2, 0);
			v1 = vadd_f32(v1, v2);
			hvec VEqualsZero = vceq_f32(v1, vdup_n_f32(0));
			hvec VEqualsInf = vceq_f32(v1, vget_low_f32(g_Infinity.v.f));
			// Reciprocal sqrt (2 iterations of Newton-Raphson)
			float32x2_t S0 = vrsqrte_f32(v1);
			float32x2_t P0 = vmul_f32(v1, S0);
			float32x2_t R0 = vrsqrts_f32(P0, S0);
			float32x2_t S1 = vmul_f32(S0, R0);
			float32x2_t P1 = vmul_f32(v1, S1);
			float32x2_t R1 = vrsqrts_f32(P1, S1);
			v2 = vmul_f32(S1, R1);
			// Normalize
			vec vResult = vmulq_f32(v.f, vcombine_f32(v2, v2));
			vResult = vbslq_f32(vec(vcombine_f32(VEqualsZero.f, VEqualsZero.f)).u, vdupq_n_f32(0), vResult.f);
			return vbslq_f32(vec(vcombine_f32(VEqualsInf.f, VEqualsInf.f)).u, g_QNaN.v.f, vResult.f);
#	endif
#endif
		}

		template<> inline vec NormalizeVec<4>(vec_f v)
		{
#ifndef MATH_USE_SIMD
			float fLength;
			vec vResult;

			fLength = LengthVec<4>(v);
			if (fLength > 0) {
				fLength = 1.0f / fLength;
			}

			vResult.f[0] = v.f[0] * fLength;
			vResult.f[1] = v.f[1] * fLength;
			vResult.f[2] = v.f[2] * fLength;
			vResult.f[3] = v.f[3] * fLength;
			return vResult;
#else
#	if defined(SIMD_SSE2)
			__m128 vLengthSq = _mm_mul_ps(v, v);
			__m128 vTemp = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			vLengthSq = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
			vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			vLengthSq = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
			__m128 vResult = _mm_sqrt_ps(vLengthSq);
			__m128 vZeroMask = _mm_setzero_ps();
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			vLengthSq = _mm_cmpneq_ps(vLengthSq, g_Infinity.v);
			vResult = _mm_div_ps(v, vResult);
			vResult = _mm_and_ps(vResult, vZeroMask);
			__m128 vTemp1 = _mm_andnot_ps(vLengthSq, g_QNaN.v);
			__m128 vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			return vResult;
#	else
			float32x4_t vTemp = vmulq_f32(v.f, v.f);
			float32x2_t v1 = vget_low_f32(vTemp);
			float32x2_t v2 = vget_high_f32(vTemp);
			v1 = vadd_f32(v1, v2);
			v1 = vpadd_f32(v1, v1);
			hvec VEqualsZero = vceq_f32(v1, vdup_n_f32(0));
			hvec VEqualsInf = vceq_f32(v1, vget_low_f32(g_Infinity.v.f));
			// Reciprocal sqrt (2 iterations of Newton-Raphson)
			float32x2_t S0 = vrsqrte_f32(v1);
			float32x2_t P0 = vmul_f32(v1, S0);
			float32x2_t R0 = vrsqrts_f32(P0, S0);
			float32x2_t S1 = vmul_f32(S0, R0);
			float32x2_t P1 = vmul_f32(v1, S1);
			float32x2_t R1 = vrsqrts_f32(P1, S1);
			v2 = vmul_f32(S1, R1);
			// Normalize
			vec vResult = vmulq_f32(v.f, vcombine_f32(v2, v2));
			vResult = vbslq_f32(vec(vcombine_f32(VEqualsZero.f, VEqualsZero.f)).u, vdupq_n_f32(0), vResult.f);
			return vbslq_f32(vec(vcombine_f32(VEqualsInf.f, VEqualsInf.f)).u, g_QNaN.v.f, vResult.f);
#	endif
#endif
		}

		MATH_CALL(float4) normalize(const float4& vec)
		{
			float4 ret;
			StoreVec<4>(reinterpret_cast<float*>(&ret), NormalizeVec<4>(LoadVec<4>(reinterpret_cast<const float*>(&vec))));
			return ret;
		}

		MATH_CALL(float3) normalize(const float3& vec)
		{
			float3 ret;
			StoreVec<3>(reinterpret_cast<float*>(&ret), NormalizeVec<3>(LoadVec<3>(reinterpret_cast<const float*>(&vec))));
			return ret;
		}

		MATH_CALL(float2) normalize(const float2& vec)
		{
			float2 ret;
			StoreVec<2>(reinterpret_cast<float*>(&ret), NormalizeVec<2>(LoadVec<2>(reinterpret_cast<const float*>(&vec))));
			return ret;
		}

		MATH_CALL(float) length(const float3& vec)
		{
			return LengthVec<3>(LoadVec<3>(reinterpret_cast<const float*>(&vec)));
		}

		MATH_CALL(float) length(const float2& vec)
		{
			return LengthVec<2>(LoadVec<2>(reinterpret_cast<const float*>(&vec)));
		}

		MATH_CALL(float) length(const float4& vec)
		{
			return LengthVec<4>(LoadVec<4>(reinterpret_cast<const float*>(&vec)));
		}

		MATH_CALL(float4) dot(const float4& vec1, const float4& vec2)
		{
			float4 ret;
			StoreVec<4>(reinterpret_cast<float*>(&ret), DotVec<4>(
				LoadVec<4>(reinterpret_cast<const float*>(&vec1)),
				LoadVec<4>(reinterpret_cast<const float*>(&vec2))));
			return ret;
		}

		MATH_CALL(float3) dot(const float3& vec1, const float3& vec2)
		{
			float3 ret;
			StoreVec<3>(reinterpret_cast<float*>(&ret), DotVec<3>(
				LoadVec<3>(reinterpret_cast<const float*>(&vec1)),
				LoadVec<3>(reinterpret_cast<const float*>(&vec2))));
			return ret;
		}

		MATH_CALL(float2) dot(const float2& vec1, const float2& vec2)
		{
			float2 ret;
			StoreVec<2>(reinterpret_cast<float*>(&ret), DotVec<2>(
				LoadVec<2>(reinterpret_cast<const float*>(&vec1)),
				LoadVec<2>(reinterpret_cast<const float*>(&vec2))));
			return ret;
		}
		
		MATH_CALL(float4) inversesqrt(const float4& vec)
		{
			float4 ret;
			StoreVec<4>(reinterpret_cast<float*>(&ret),
				InverseSqrtVec(LoadVec<4>(reinterpret_cast<const float*>(&vec))));
			return ret;
		}

		MATH_CALL(float3) inversesqrt(const float3& vec)
		{
			float3 ret;
			StoreVec<3>(reinterpret_cast<float*>(&ret),
				InverseSqrtVec(LoadVec<3>(reinterpret_cast<const float*>(&vec))));
			return ret;
		}

		MATH_CALL(float2) inversesqrt(const float2& vec)
		{
			float2 ret;
			StoreVec<2>(reinterpret_cast<float*>(&ret),
				InverseSqrtVec(LoadVec<2>(reinterpret_cast<const float*>(&vec))));
			return ret;
		}

		MATH_CALL(float4) sqrt(const float4& vec)
		{
			float4 ret;
			StoreVec<4>(reinterpret_cast<float*>(&ret), SqrtVec(LoadVec<4>(reinterpret_cast<const float*>(&vec))));
			return ret;
		}

		MATH_CALL(float3) sqrt(const float3& vec)
		{
			float3 ret;
			StoreVec<3>(reinterpret_cast<float*>(&ret), SqrtVec(LoadVec<3>(reinterpret_cast<const float*>(&vec))));
			return ret;
		}

		MATH_CALL(float2) sqrt(const float2& vec)
		{
			float2 ret;
			StoreVec<2>(reinterpret_cast<float*>(&ret), SqrtVec(LoadVec<2>(reinterpret_cast<const float*>(&vec))));
			return ret;
		}

		MATH_CALL(matrix) matrix::Translation(float x, float y, float z)
		{
			matrix M;
			M.v[0] = g_IdentityR0.v;
			M.v[1] = g_IdentityR1.v;
			M.v[2] = g_IdentityR2.v;
			M.fa[3].f[0] = x;
			M.fa[3].f[1] = y;
			M.fa[3].f[2] = z;
			M.fa[3].f[3] = 1.f;
			return M;
		}

		MATH_CALL(matrix) matrix::Scaling(float x, float y, float z)
		{
			matrix M;
			M.fa[0].f[0] = x;
			M.fa[0].f[1] = 0.f;
			M.fa[0].f[2] = 0.f;
			M.fa[0].f[3] = 0.f;
			M.fa[1].f[0] = 0.f;
			M.fa[1].f[1] = y;
			M.fa[1].f[2] = 0.f;
			M.fa[1].f[3] = 0.f;
			M.fa[2].f[0] = 0.f;
			M.fa[2].f[1] = 0.f;
			M.fa[2].f[2] = z;
			M.fa[2].f[3] = 0.f;
			M.v[3] = g_IdentityR3.v;
			return M;
		}
		
		bool matrix::IsIdentity() const
		{
#ifndef MATH_USE_SIMD
			const uint32_t *pWork = (const uint32_t*)(&this->v[0]);
			uint32_t uOne = pWork[0] ^ 0x3F800000U;
			uint32_t uZero = pWork[1];
			uZero |= pWork[2];
			uZero |= pWork[3];
			uZero |= pWork[4];
			uOne |= pWork[5] ^ 0x3F800000U;
			uZero |= pWork[6];
			uZero |= pWork[7];
			uZero |= pWork[8];
			uZero |= pWork[9];
			uOne |= pWork[10] ^ 0x3F800000U;
			uZero |= pWork[11];
			uZero |= pWork[12];
			uZero |= pWork[13];
			uZero |= pWork[14];
			uOne |= pWork[15] ^ 0x3F800000U;
			uZero &= 0x7FFFFFFF;
			uOne |= uZero;
			return (uOne == 0);
#else
#	if defined(SIMD_SSE2)
			__m128 vTemp1 = _mm_cmpeq_ps(v[0], g_IdentityR0.v);
			__m128 vTemp2 = _mm_cmpeq_ps(v[1], g_IdentityR1.v);
			__m128 vTemp3 = _mm_cmpeq_ps(v[2], g_IdentityR2.v);
			__m128 vTemp4 = _mm_cmpeq_ps(v[3], g_IdentityR3.v);
			vTemp1 = _mm_and_ps(vTemp1, vTemp2);
			vTemp3 = _mm_and_ps(vTemp3, vTemp4);
			vTemp1 = _mm_and_ps(vTemp1, vTemp3);
			return (_mm_movemask_ps(vTemp1) == 0x0f);
#	else
			vec vTemp1 = vceqq_f32(v[0].f, g_IdentityR0.v.f);
			vec vTemp2 = vceqq_f32(v[1].f, g_IdentityR1.v.f);
			vec vTemp3 = vceqq_f32(v[2].f, g_IdentityR2.v.f);
			vec vTemp4 = vceqq_f32(v[3].f, g_IdentityR3.v.f);
			vTemp1 = vandq_u32(vTemp1.u, vTemp2.u);
			vTemp3 = vandq_u32(vTemp3.u, vTemp4.u);
			vTemp1 = vandq_u32(vTemp1.u, vTemp3.u);
			uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vTemp1.b), vget_high_u8(vTemp1.b));

			vec Temp = vec(vzip_u16(hvec(vTemp.val[0]).sa4, hvec(vTemp.val[1]).sa4));
			uint32_t r = vget_lane_u32(hvec(Temp.ba2.val[1]).u, 1);
			return (r == 0xFFFFFFFFU);
#	endif
#endif
		}
		
		bool matrix::IsNaN() const
		{
#ifndef MATH_USE_SIMD
			size_t i = 16;
			const uint32_t *pWork = (const uint32_t *)(&this->v[0]);
			do {
				uint32_t uTest = pWork[0];
				uTest &= 0x7FFFFFFFU;
				uTest -= 0x7F800001U;
				if (uTest < 0x007FFFFFU) {
					break;      // NaN found
				}
				++pWork;        // Next entry
			} while (--i);
			return (i != 0);      // i == 0 if nothing matched
#else
#	if defined(SIMD_SSE2)
			__m128 vX = v[0];
			__m128 vY = v[1];
			__m128 vZ = v[2];
			__m128 vW = v[3];
			vX = _mm_cmpneq_ps(vX, vX);
			vY = _mm_cmpneq_ps(vY, vY);
			vZ = _mm_cmpneq_ps(vZ, vZ);
			vW = _mm_cmpneq_ps(vW, vW);
			vX = _mm_or_ps(vX, vZ);
			vY = _mm_or_ps(vY, vW);
			vX = _mm_or_ps(vX, vY);
			return (_mm_movemask_ps(vX) != 0);
#	else
			// Load in registers
			vec vX = v[0];
			vec vY = v[1];
			vec vZ = v[2];
			vec vW = v[3];
			// Test themselves to check for NaN
			vX = vmvnq_u32(vec(vceqq_f32(vX.f, vX.f)).u);
			vY = vmvnq_u32(vec(vceqq_f32(vY.f, vY.f)).u);
			vZ = vmvnq_u32(vec(vceqq_f32(vZ.f, vZ.f)).u);
			vW = vmvnq_u32(vec(vceqq_f32(vW.f, vW.f)).u);
			// Or all the results
			vX = vorrq_u32(vX.u, vZ.u);
			vY = vorrq_u32(vY.u, vW.u);
			vX = vorrq_u32(vX.u, vY.u);
			// If any tested true, return true
			uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vX.b), vget_high_u8(vX.b));
			uint16x4x2_t Temp = vzip_u16(hvec(vTemp.val[0]).sa4, hvec(vTemp.val[1]).sa4);
			uint32_t r = vget_lane_u32(hvec(Temp.val[1]).u, 1);
			return (r != 0);
#	endif
#endif
		}
		
		bool matrix::IsInfinite() const
		{
#ifndef MATH_USE_SIMD
			size_t i = 16;
			const uint32_t *pWork = (const uint32_t *)(&v[0]);
			do {
				// Fetch value into integer unit
				uint32_t uTest = pWork[0];
				// Remove sign
				uTest &= 0x7FFFFFFFU;
				// INF is 0x7F800000
				if (uTest == 0x7F800000U) {
					break;      // INF found
				}
				++pWork;        // Next entry
			} while (--i);
			return (i != 0);      // i == 0 if nothing matched
#else
#	if defined(SIMD_SSE2)
			__m128 vTemp1 = _mm_and_ps(v[0], g_AbsMask.v);
			__m128 vTemp2 = _mm_and_ps(v[1], g_AbsMask.v);
			__m128 vTemp3 = _mm_and_ps(v[2], g_AbsMask.v);
			__m128 vTemp4 = _mm_and_ps(v[3], g_AbsMask.v);
			// Compare to infinity
			vTemp1 = _mm_cmpeq_ps(vTemp1, g_Infinity.v);
			vTemp2 = _mm_cmpeq_ps(vTemp2, g_Infinity.v);
			vTemp3 = _mm_cmpeq_ps(vTemp3, g_Infinity.v);
			vTemp4 = _mm_cmpeq_ps(vTemp4, g_Infinity.v);
			// Or the answers together
			vTemp1 = _mm_or_ps(vTemp1, vTemp2);
			vTemp3 = _mm_or_ps(vTemp3, vTemp4);
			vTemp1 = _mm_or_ps(vTemp1, vTemp3);
			// If any are infinity, the signs are true.
			return (_mm_movemask_ps(vTemp1) != 0);
#	else
			vec vTemp1 = vandq_u32(v[0].u, g_AbsMask.v.u);
			vec vTemp2 = vandq_u32(v[1].u, g_AbsMask.v.u);
			vec vTemp3 = vandq_u32(v[2].u, g_AbsMask.v.u);
			vec vTemp4 = vandq_u32(v[3].u, g_AbsMask.v.u);
			// Compare to infinity
			vTemp1 = vceqq_f32(vTemp1.f, g_Infinity.v.f);
			vTemp2 = vceqq_f32(vTemp2.f, g_Infinity.v.f);
			vTemp3 = vceqq_f32(vTemp3.f, g_Infinity.v.f);
			vTemp4 = vceqq_f32(vTemp4.f, g_Infinity.v.f);
			// Or the answers together
			vTemp1 = vorrq_u32(vTemp1.u, vTemp2.u);
			vTemp3 = vorrq_u32(vTemp3.u, vTemp4.u);
			vTemp1 = vorrq_u32(vTemp1.u, vTemp3.u);
			// If any are infinity, the signs are true.
			uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vTemp1.b), vget_high_u8(vTemp1.b));
			uint16x4x2_t Temp = vzip_u16(hvec(vTemp.val[0]).sa4, hvec(vTemp.val[1]).sa4);
			uint32_t r = vget_lane_u32(hvec(Temp.val[1]).u, 1);
			return (r != 0);
#	endif
#endif
		}
		
		MATH_CALL(matrix&) matrix::operator=(const matrix & a)
		{
			this->v[0] = a.v[0];
			this->v[1] = a.v[1];
			this->v[2] = a.v[2];
			this->v[3] = a.v[3];
			return *this;
		}
		
		MATH_CALL(matrix) matrix::TransposeMatrix() const
		{
#ifndef MATH_USE_SIMD
			matrix MT;

			MT.v[0].f[1] = this->v[1].f[0];
			MT.v[1].f[0] = this->v[0].f[1];
			MT.v[0].f[2] = this->v[2].f[0];
			MT.v[2].f[0] = this->v[0].f[2];
			MT.v[0].f[3] = this->v[3].f[0];
			MT.v[3].f[0] = this->v[0].f[3];

			MT.v[2].f[1] = this->v[1].f[2];
			MT.v[1].f[2] = this->v[2].f[1];
			MT.v[3].f[1] = this->v[1].f[3];
			MT.v[1].f[3] = this->v[3].f[1];
			MT.v[3].f[2] = this->v[2].f[3];
			MT.v[2].f[3] = this->v[3].f[2];

			return MT;
#else
#	if defined(SIMD_SSE2)
			__m128 vTemp1 = _mm_shuffle_ps(v[0], v[1], _MM_SHUFFLE(1, 0, 1, 0));
			__m128 vTemp3 = _mm_shuffle_ps(v[0], v[1], _MM_SHUFFLE(3, 2, 3, 2));
			__m128 vTemp2 = _mm_shuffle_ps(v[2], v[3], _MM_SHUFFLE(1, 0, 1, 0));
			__m128 vTemp4 = _mm_shuffle_ps(v[2], v[3], _MM_SHUFFLE(3, 2, 3, 2));
			matrix mResult;
			mResult.v[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
			mResult.v[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
			mResult.v[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
			mResult.v[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
			return mResult;
#	else
			float32x4x2_t P0 = vzipq_f32(v[0].f, v[2].f);
			float32x4x2_t P1 = vzipq_f32(v[1].f, v[3].f);

			float32x4x2_t T0 = vzipq_f32(P0.val[0], P1.val[0]);
			float32x4x2_t T1 = vzipq_f32(P0.val[1], P1.val[1]);

			matrix mResult;
			mResult.v[0] = T0.val[0];
			mResult.v[1] = T0.val[1];
			mResult.v[2] = T1.val[0];
			mResult.v[3] = T1.val[1];
			return mResult;
#	endif
#endif
		}
		
		void matrix::Transpose()
		{
#ifndef MATH_USE_SIMD
			std::swap(this->v[0].f[1], this->v[1].f[0]);
			std::swap(this->v[0].f[2], this->v[2].f[0]);
			std::swap(this->v[0].f[3], this->v[3].f[0]);

			std::swap(this->v[2].f[1], this->v[1].f[2]);
			std::swap(this->v[3].f[1], this->v[1].f[3]);
			std::swap(this->v[3].f[2], this->v[2].f[3]);
#else
#	if defined(SIMD_SSE2)
			__m128 vTemp1 = _mm_shuffle_ps(v[0], v[1], _MM_SHUFFLE(1, 0, 1, 0));
			__m128 vTemp3 = _mm_shuffle_ps(v[0], v[1], _MM_SHUFFLE(3, 2, 3, 2));
			__m128 vTemp2 = _mm_shuffle_ps(v[2], v[3], _MM_SHUFFLE(1, 0, 1, 0));
			__m128 vTemp4 = _mm_shuffle_ps(v[2], v[3], _MM_SHUFFLE(3, 2, 3, 2));
			v[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
			v[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
			v[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
			v[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
#	else
			float32x4x2_t P0 = vzipq_f32(v[0].f, v[2].f);
			float32x4x2_t P1 = vzipq_f32(v[1].f, v[3].f);
			float32x4x2_t T0 = vzipq_f32(P0.val[0], P1.val[0]);
			float32x4x2_t T1 = vzipq_f32(P0.val[1], P1.val[1]);
			
			v[0] = T0.val[0];
			v[1] = T0.val[1];
			v[2] = T1.val[0];
			v[3] = T1.val[1];
#	endif
#endif
		}
		
		MATH_CALL(matrix) matrix::InverseMatrix() const
		{
#if !defined(MATH_USE_SIMD) || defined(SIMD_NEON)
			matrix MT = TransposeMatrix();

			vec V0[4], V1[4];
			V0[0] = Swizzle<0, 0, 1, 1>(MT.v[2]);
			V1[0] = Swizzle<2, 3, 2, 3>(MT.v[3]);
			V0[1] = Swizzle<0, 0, 1, 1>(MT.v[0]);
			V1[1] = Swizzle<2, 3, 2, 3>(MT.v[1]);
			V0[2] = Permute<0, 2, 4, 6>(MT.v[2], MT.v[0]);
			V1[2] = Permute<1, 3, 5, 7>(MT.v[3], MT.v[1]);

			vec D0 = MultiplyVec(V0[0], V1[0]);
			vec D1 = MultiplyVec(V0[1], V1[1]);
			vec D2 = MultiplyVec(V0[2], V1[2]);

			V0[0] = Swizzle<2, 3, 2, 3>(MT.v[2]);
			V1[0] = Swizzle<0, 0, 1, 1>(MT.v[3]);
			V0[1] = Swizzle<2, 3, 2, 3>(MT.v[0]);
			V1[1] = Swizzle<0, 0, 1, 1>(MT.v[1]);
			V0[2] = Permute<1, 3, 5, 7>(MT.v[2], MT.v[0]);
			V1[2] = Permute<0, 2, 4, 6>(MT.v[3], MT.v[1]);

			D0 = SubVec(D0, MultiplyVec(V0[0], V1[0]));
			D1 = SubVec(D1, MultiplyVec(V0[1], V1[1]));
			D2 = SubVec(D2, MultiplyVec(V0[2], V1[2]));

			V0[0] = Swizzle<1, 2, 0, 1>(MT.v[1]);
			V1[0] = Permute<5, 1, 3, 0>(D0, D2);
			V0[1] = Swizzle<2, 0, 1, 0>(MT.v[0]);
			V1[1] = Permute<3, 5, 1, 2>(D0, D2);
			V0[2] = Swizzle<1, 2, 0, 1>(MT.v[3]);
			V1[2] = Permute<7, 1, 3, 0>(D1, D2);
			V0[3] = Swizzle<2, 0, 1, 0>(MT.v[2]);
			V1[3] = Permute<3, 7, 1, 2>(D1, D2);

			vec C0 = MultiplyVec(V0[0], V1[0]);
			vec C2 = MultiplyVec(V0[1], V1[1]);
			vec C4 = MultiplyVec(V0[2], V1[2]);
			vec C6 = MultiplyVec(V0[3], V1[3]);

			V0[0] = Swizzle<2, 3, 1, 2>(MT.v[1]);
			V1[0] = Permute<3, 0, 1, 4>(D0, D2);
			V0[1] = Swizzle<3, 2, 3, 1>(MT.v[0]);
			V1[1] = Permute<2, 1, 4, 0>(D0, D2);
			V0[2] = Swizzle<2, 3, 1, 2>(MT.v[3]);
			V1[2] = Permute<3, 0, 1, 6>(D1, D2);
			V0[3] = Swizzle<3, 2, 3, 1>(MT.v[2]);
			V1[3] = Permute<2, 1, 6, 0>(D1, D2);

			C0 = SubVec(C0, MultiplyVec(V0[0], V1[0]));
			C2 = SubVec(C2, MultiplyVec(V0[1], V1[1]));
			C4 = SubVec(C4, MultiplyVec(V0[2], V1[2]));
			C6 = SubVec(C6, MultiplyVec(V0[3], V1[3]));

			V0[0] = Swizzle<3, 0, 3, 0>(MT.v[1]);
			V1[0] = Permute<2, 5, 4, 2>(D0, D2);
			V0[1] = Swizzle<1, 3, 0, 2>(MT.v[0]);
			V1[1] = Permute<5, 0, 3, 4>(D0, D2);
			V0[2] = Swizzle<3, 0, 3, 0>(MT.v[3]);
			V1[2] = Permute<2, 7, 6, 2>(D1, D2);
			V0[3] = Swizzle<1, 3, 0, 2>(MT.v[2]);
			V1[3] = Permute<7, 0, 3, 6>(D1, D2);

			vec C1 = SubVec(C0, MultiplyVec(V0[0], V1[0]));
			C0 = AddVec(MultiplyVec(V0[0], V1[0]), C0);
			vec C3 = AddVec(MultiplyVec(V0[1], V1[1]), C2);
			C2 = SubVec(C2, MultiplyVec(V0[1], V1[1]));
			vec C5 = SubVec(C4, MultiplyVec(V0[2], V1[2]));
			C4 = AddVec(MultiplyVec(V0[2], V1[2]), C4);
			vec C7 = AddVec(MultiplyVec(V0[3], V1[3]), C6);
			C6 = SubVec(C6, MultiplyVec(V0[3], V1[3]));

			matrix R;

			VEC_GETU(R.v[0].u, 0) = (VEC_GETU(C0.u, 0) & ~VEC_GETU(g_Select0101.v.u, 0)) | (VEC_GETU(C1.u, 0) & VEC_GETU(g_Select0101.v.u, 0));
			VEC_GETU(R.v[0].u, 1) = (VEC_GETU(C0.u, 1) & ~VEC_GETU(g_Select0101.v.u, 1)) | (VEC_GETU(C1.u, 1) & VEC_GETU(g_Select0101.v.u, 1));
			VEC_GETU(R.v[0].u, 2) = (VEC_GETU(C0.u, 2) & ~VEC_GETU(g_Select0101.v.u, 2)) | (VEC_GETU(C1.u, 2) & VEC_GETU(g_Select0101.v.u, 2));
			VEC_GETU(R.v[0].u, 3) = (VEC_GETU(C0.u, 3) & ~VEC_GETU(g_Select0101.v.u, 3)) | (VEC_GETU(C1.u, 3) & VEC_GETU(g_Select0101.v.u, 3));

			VEC_GETU(R.v[1].u, 0) = (VEC_GETU(C2.u, 0) & ~VEC_GETU(g_Select0101.v.u, 0)) | (VEC_GETU(C3.u, 0) & VEC_GETU(g_Select0101.v.u, 0));
			VEC_GETU(R.v[1].u, 1) = (VEC_GETU(C2.u, 1) & ~VEC_GETU(g_Select0101.v.u, 1)) | (VEC_GETU(C3.u, 1) & VEC_GETU(g_Select0101.v.u, 1));
			VEC_GETU(R.v[1].u, 2) = (VEC_GETU(C2.u, 2) & ~VEC_GETU(g_Select0101.v.u, 2)) | (VEC_GETU(C3.u, 2) & VEC_GETU(g_Select0101.v.u, 2));
			VEC_GETU(R.v[1].u, 3) = (VEC_GETU(C2.u, 3) & ~VEC_GETU(g_Select0101.v.u, 3)) | (VEC_GETU(C3.u, 3) & VEC_GETU(g_Select0101.v.u, 3));

			VEC_GETU(R.v[2].u, 0) = (VEC_GETU(C4.u, 0) & ~VEC_GETU(g_Select0101.v.u, 0)) | (VEC_GETU(C5.u, 0) & VEC_GETU(g_Select0101.v.u, 0));
			VEC_GETU(R.v[2].u, 1) = (VEC_GETU(C4.u, 1) & ~VEC_GETU(g_Select0101.v.u, 1)) | (VEC_GETU(C5.u, 1) & VEC_GETU(g_Select0101.v.u, 1));
			VEC_GETU(R.v[2].u, 2) = (VEC_GETU(C4.u, 2) & ~VEC_GETU(g_Select0101.v.u, 2)) | (VEC_GETU(C5.u, 2) & VEC_GETU(g_Select0101.v.u, 2));
			VEC_GETU(R.v[2].u, 3) = (VEC_GETU(C4.u, 3) & ~VEC_GETU(g_Select0101.v.u, 3)) | (VEC_GETU(C5.u, 3) & VEC_GETU(g_Select0101.v.u, 3));

			VEC_GETU(R.v[3].u, 0) = (VEC_GETU(C6.u, 0) & ~VEC_GETU(g_Select0101.v.u, 0)) | (VEC_GETU(C7.u, 0) & VEC_GETU(g_Select0101.v.u, 0));
			VEC_GETU(R.v[3].u, 1) = (VEC_GETU(C6.u, 1) & ~VEC_GETU(g_Select0101.v.u, 1)) | (VEC_GETU(C7.u, 1) & VEC_GETU(g_Select0101.v.u, 1));
			VEC_GETU(R.v[3].u, 2) = (VEC_GETU(C6.u, 2) & ~VEC_GETU(g_Select0101.v.u, 2)) | (VEC_GETU(C7.u, 2) & VEC_GETU(g_Select0101.v.u, 2));
			VEC_GETU(R.v[3].u, 3) = (VEC_GETU(C6.u, 3) & ~VEC_GETU(g_Select0101.v.u, 3)) | (VEC_GETU(C7.u, 3) & VEC_GETU(g_Select0101.v.u, 3));

			vec Determinant = DotVec<4>(R.v[0], MT.v[0]);

			vec Reciprocal;
			VEC_GETF(Reciprocal.f, 0) = 1.f / VEC_GETF(Determinant.f, 0);
			VEC_GETF(Reciprocal.f, 1) = 1.f / VEC_GETF(Determinant.f, 1);
			VEC_GETF(Reciprocal.f, 2) = 1.f / VEC_GETF(Determinant.f, 2);
			VEC_GETF(Reciprocal.f, 3) = 1.f / VEC_GETF(Determinant.f, 3);

			matrix Result;
			Result.v[0] = MultiplyVec(R.v[0], Reciprocal);
			Result.v[1] = MultiplyVec(R.v[1], Reciprocal);
			Result.v[2] = MultiplyVec(R.v[2], Reciprocal);
			Result.v[3] = MultiplyVec(R.v[3], Reciprocal);
			return Result;
#else
#	if defined(SIMD_SSE2)
			__m128 vTemp1 = _mm_shuffle_ps(v[0], v[1], _MM_SHUFFLE(1, 0, 1, 0));
			__m128 vTemp3 = _mm_shuffle_ps(v[0], v[1], _MM_SHUFFLE(3, 2, 3, 2));
			__m128 vTemp2 = _mm_shuffle_ps(v[2], v[3], _MM_SHUFFLE(1, 0, 1, 0));
			__m128 vTemp4 = _mm_shuffle_ps(v[2], v[3], _MM_SHUFFLE(3, 2, 3, 2));
			__m128 MT[4];
			MT[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
			MT[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
			MT[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
			MT[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));

			__m128 V00 = _mm_shuffle_ps(MT[2], MT[2], _MM_SHUFFLE(1, 1, 0, 0));
			__m128 V10 = _mm_shuffle_ps(MT[3], MT[3], _MM_SHUFFLE(3, 2, 3, 2));
			__m128 V01 = _mm_shuffle_ps(MT[0], MT[0], _MM_SHUFFLE(1, 1, 0, 0));
			__m128 V11 = _mm_shuffle_ps(MT[1], MT[1], _MM_SHUFFLE(3, 2, 3, 2));
			__m128 V02 = _mm_shuffle_ps(MT[2], MT[0], _MM_SHUFFLE(2, 0, 2, 0));
			__m128 V12 = _mm_shuffle_ps(MT[3], MT[1], _MM_SHUFFLE(3, 1, 3, 1));

			__m128 D0 = _mm_mul_ps(V00, V10);
			__m128 D1 = _mm_mul_ps(V01, V11);
			__m128 D2 = _mm_mul_ps(V02, V12);

			V00 = _mm_shuffle_ps(MT[2], MT[2], _MM_SHUFFLE(3, 2, 3, 2));
			V10 = _mm_shuffle_ps(MT[3], MT[3], _MM_SHUFFLE(1, 1, 0, 0));
			V01 = _mm_shuffle_ps(MT[0], MT[0], _MM_SHUFFLE(3, 2, 3, 2));
			V11 = _mm_shuffle_ps(MT[1], MT[1], _MM_SHUFFLE(1, 1, 0, 0));
			V02 = _mm_shuffle_ps(MT[2], MT[0], _MM_SHUFFLE(3, 1, 3, 1));
			V12 = _mm_shuffle_ps(MT[3], MT[1], _MM_SHUFFLE(2, 0, 2, 0));

			V00 = _mm_mul_ps(V00, V10);
			V01 = _mm_mul_ps(V01, V11);
			V02 = _mm_mul_ps(V02, V12);
			D0 = _mm_sub_ps(D0, V00);
			D1 = _mm_sub_ps(D1, V01);
			D2 = _mm_sub_ps(D2, V02);
			// V11 = D0Y,D0W,D2Y,D2Y
			V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 1, 3, 1));
			V00 = _mm_shuffle_ps(MT[1], MT[1], _MM_SHUFFLE(1, 0, 2, 1));
			V10 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(0, 3, 0, 2));
			V01 = _mm_shuffle_ps(MT[0], MT[0], _MM_SHUFFLE(0, 1, 0, 2));
			V11 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(2, 1, 2, 1));
			// V13 = D1Y,D1W,D2W,D2W
			__m128 V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 3, 3, 1));
			V02 = _mm_shuffle_ps(MT[3], MT[3], _MM_SHUFFLE(1, 0, 2, 1));
			V12 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(0, 3, 0, 2));
			__m128 V03 = _mm_shuffle_ps(MT[2], MT[2], _MM_SHUFFLE(0, 1, 0, 2));
			V13 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(2, 1, 2, 1));

			__m128 C0 = _mm_mul_ps(V00, V10);
			__m128 C2 = _mm_mul_ps(V01, V11);
			__m128 C4 = _mm_mul_ps(V02, V12);
			__m128 C6 = _mm_mul_ps(V03, V13);

			// V11 = D0X,D0Y,D2X,D2X
			V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(0, 0, 1, 0));
			V00 = _mm_shuffle_ps(MT[1], MT[1], _MM_SHUFFLE(2, 1, 3, 2));
			V10 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(2, 1, 0, 3));
			V01 = _mm_shuffle_ps(MT[0], MT[0], _MM_SHUFFLE(1, 3, 2, 3));
			V11 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(0, 2, 1, 2));
			// V13 = D1X,D1Y,D2Z,D2Z
			V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(2, 2, 1, 0));
			V02 = _mm_shuffle_ps(MT[3], MT[3], _MM_SHUFFLE(2, 1, 3, 2));
			V12 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(2, 1, 0, 3));
			V03 = _mm_shuffle_ps(MT[2], MT[2], _MM_SHUFFLE(1, 3, 2, 3));
			V13 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(0, 2, 1, 2));

			V00 = _mm_mul_ps(V00, V10);
			V01 = _mm_mul_ps(V01, V11);
			V02 = _mm_mul_ps(V02, V12);
			V03 = _mm_mul_ps(V03, V13);
			C0 = _mm_sub_ps(C0, V00);
			C2 = _mm_sub_ps(C2, V01);
			C4 = _mm_sub_ps(C4, V02);
			C6 = _mm_sub_ps(C6, V03);

			V00 = _mm_shuffle_ps(MT[1], MT[1], _MM_SHUFFLE(0, 3, 0, 3));
			// V10 = D0Z,D0Z,D2X,D2Y
			V10 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 2, 2));
			V10 = _mm_shuffle_ps(V10, V10, _MM_SHUFFLE(0, 2, 3, 0));
			V01 = _mm_shuffle_ps(MT[0], MT[0], _MM_SHUFFLE(2, 0, 3, 1));
			// V11 = D0X,D0W,D2X,D2Y
			V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 3, 0));
			V11 = _mm_shuffle_ps(V11, V11, _MM_SHUFFLE(2, 1, 0, 3));
			V02 = _mm_shuffle_ps(MT[3], MT[3], _MM_SHUFFLE(0, 3, 0, 3));
			// V12 = D1Z,D1Z,D2Z,D2W
			V12 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 2, 2));
			V12 = _mm_shuffle_ps(V12, V12, _MM_SHUFFLE(0, 2, 3, 0));
			V03 = _mm_shuffle_ps(MT[2], MT[2], _MM_SHUFFLE(2, 0, 3, 1));
			// V13 = D1X,D1W,D2Z,D2W
			V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 3, 0));
			V13 = _mm_shuffle_ps(V13, V13, _MM_SHUFFLE(2, 1, 0, 3));

			V00 = _mm_mul_ps(V00, V10);
			V01 = _mm_mul_ps(V01, V11);
			V02 = _mm_mul_ps(V02, V12);
			V03 = _mm_mul_ps(V03, V13);
			__m128 C1 = _mm_sub_ps(C0, V00);
			C0 = _mm_add_ps(C0, V00);
			__m128 C3 = _mm_add_ps(C2, V01);
			C2 = _mm_sub_ps(C2, V01);
			__m128 C5 = _mm_sub_ps(C4, V02);
			C4 = _mm_add_ps(C4, V02);
			__m128 C7 = _mm_add_ps(C6, V03);
			C6 = _mm_sub_ps(C6, V03);

			C0 = _mm_shuffle_ps(C0, C1, _MM_SHUFFLE(3, 1, 2, 0));
			C2 = _mm_shuffle_ps(C2, C3, _MM_SHUFFLE(3, 1, 2, 0));
			C4 = _mm_shuffle_ps(C4, C5, _MM_SHUFFLE(3, 1, 2, 0));
			C6 = _mm_shuffle_ps(C6, C7, _MM_SHUFFLE(3, 1, 2, 0));
			C0 = _mm_shuffle_ps(C0, C0, _MM_SHUFFLE(3, 1, 2, 0));
			C2 = _mm_shuffle_ps(C2, C2, _MM_SHUFFLE(3, 1, 2, 0));
			C4 = _mm_shuffle_ps(C4, C4, _MM_SHUFFLE(3, 1, 2, 0));
			C6 = _mm_shuffle_ps(C6, C6, _MM_SHUFFLE(3, 1, 2, 0));
			__m128 vTemp = DotVec<4>(C0, MT[0]);
			vTemp = _mm_div_ps(g_One.v, vTemp);
			matrix mResult;
			mResult.v[0] = _mm_mul_ps(C0, vTemp);
			mResult.v[1] = _mm_mul_ps(C2, vTemp);
			mResult.v[2] = _mm_mul_ps(C4, vTemp);
			mResult.v[3] = _mm_mul_ps(C6, vTemp);

			return mResult;
#	else

#	endif
#endif
		}
		
		void matrix::Inverse()
		{
#if !defined(MATH_USE_SIMD) || defined(SIMD_NEON)
			matrix MT = TransposeMatrix();

			vec V0[4], V1[4];
			V0[0] = Swizzle<0, 0, 1, 1>(MT.v[2]);
			V1[0] = Swizzle<2, 3, 2, 3>(MT.v[3]);
			V0[1] = Swizzle<0, 0, 1, 1>(MT.v[0]);
			V1[1] = Swizzle<2, 3, 2, 3>(MT.v[1]);
			V0[2] = Permute<0, 2, 4, 6>(MT.v[2], MT.v[0]);
			V1[2] = Permute<1, 3, 5, 7>(MT.v[3], MT.v[1]);

			vec D0 = MultiplyVec(V0[0], V1[0]);
			vec D1 = MultiplyVec(V0[1], V1[1]);
			vec D2 = MultiplyVec(V0[2], V1[2]);

			V0[0] = Swizzle<2, 3, 2, 3>(MT.v[2]);
			V1[0] = Swizzle<0, 0, 1, 1>(MT.v[3]);
			V0[1] = Swizzle<2, 3, 2, 3>(MT.v[0]);
			V1[1] = Swizzle<0, 0, 1, 1>(MT.v[1]);
			V0[2] = Permute<1, 3, 5, 7>(MT.v[2], MT.v[0]);
			V1[2] = Permute<0, 2, 4, 6>(MT.v[3], MT.v[1]);

			D0 = SubVec(D0, MultiplyVec(V0[0], V1[0]));
			D1 = SubVec(D1, MultiplyVec(V0[1], V1[1]));
			D2 = SubVec(D2, MultiplyVec(V0[2], V1[2]));

			V0[0] = Swizzle<1, 2, 0, 1>(MT.v[1]);
			V1[0] = Permute<5, 1, 3, 0>(D0, D2);
			V0[1] = Swizzle<2, 0, 1, 0>(MT.v[0]);
			V1[1] = Permute<3, 5, 1, 2>(D0, D2);
			V0[2] = Swizzle<1, 2, 0, 1>(MT.v[3]);
			V1[2] = Permute<7, 1, 3, 0>(D1, D2);
			V0[3] = Swizzle<2, 0, 1, 0>(MT.v[2]);
			V1[3] = Permute<3, 7, 1, 2>(D1, D2);

			vec C0 = MultiplyVec(V0[0], V1[0]);
			vec C2 = MultiplyVec(V0[1], V1[1]);
			vec C4 = MultiplyVec(V0[2], V1[2]);
			vec C6 = MultiplyVec(V0[3], V1[3]);

			V0[0] = Swizzle<2, 3, 1, 2>(MT.v[1]);
			V1[0] = Permute<3, 0, 1, 4>(D0, D2);
			V0[1] = Swizzle<3, 2, 3, 1>(MT.v[0]);
			V1[1] = Permute<2, 1, 4, 0>(D0, D2);
			V0[2] = Swizzle<2, 3, 1, 2>(MT.v[3]);
			V1[2] = Permute<3, 0, 1, 6>(D1, D2);
			V0[3] = Swizzle<3, 2, 3, 1>(MT.v[2]);
			V1[3] = Permute<2, 1, 6, 0>(D1, D2);

			C0 = SubVec(C0, MultiplyVec(V0[0], V1[0]));
			C2 = SubVec(C2, MultiplyVec(V0[1], V1[1]));
			C4 = SubVec(C4, MultiplyVec(V0[2], V1[2]));
			C6 = SubVec(C6, MultiplyVec(V0[3], V1[3]));

			V0[0] = Swizzle<3, 0, 3, 0>(MT.v[1]);
			V1[0] = Permute<2, 5, 4, 2>(D0, D2);
			V0[1] = Swizzle<1, 3, 0, 2>(MT.v[0]);
			V1[1] = Permute<5, 0, 3, 4>(D0, D2);
			V0[2] = Swizzle<3, 0, 3, 0>(MT.v[3]);
			V1[2] = Permute<2, 7, 6, 2>(D1, D2);
			V0[3] = Swizzle<1, 3, 0, 2>(MT.v[2]);
			V1[3] = Permute<7, 0, 3, 6>(D1, D2);

			vec C1 = SubVec(C0, MultiplyVec(V0[0], V1[0]));
			C0 = AddVec(MultiplyVec(V0[0], V1[0]), C0);
			vec C3 = AddVec(MultiplyVec(V0[1], V1[1]), C2);
			C2 = SubVec(C2, MultiplyVec(V0[1], V1[1]));
			vec C5 = SubVec(C4, MultiplyVec(V0[2], V1[2]));
			C4 = AddVec(MultiplyVec(V0[2], V1[2]), C4);
			vec C7 = AddVec(MultiplyVec(V0[3], V1[3]), C6);
			C6 = SubVec(C6, MultiplyVec(V0[3], V1[3]));

			matrix R;

			VEC_GETU(R.v[0].u, 0) = (VEC_GETU(C0.u, 0) & ~VEC_GETU(g_Select0101.v.u, 0)) | (VEC_GETU(C1.u, 0) & VEC_GETU(g_Select0101.v.u, 0));
			VEC_GETU(R.v[0].u, 1) = (VEC_GETU(C0.u, 1) & ~VEC_GETU(g_Select0101.v.u, 1)) | (VEC_GETU(C1.u, 1) & VEC_GETU(g_Select0101.v.u, 1));
			VEC_GETU(R.v[0].u, 2) = (VEC_GETU(C0.u, 2) & ~VEC_GETU(g_Select0101.v.u, 2)) | (VEC_GETU(C1.u, 2) & VEC_GETU(g_Select0101.v.u, 2));
			VEC_GETU(R.v[0].u, 3) = (VEC_GETU(C0.u, 3) & ~VEC_GETU(g_Select0101.v.u, 3)) | (VEC_GETU(C1.u, 3) & VEC_GETU(g_Select0101.v.u, 3));

			VEC_GETU(R.v[1].u, 0) = (VEC_GETU(C2.u, 0) & ~VEC_GETU(g_Select0101.v.u, 0)) | (VEC_GETU(C3.u, 0) & VEC_GETU(g_Select0101.v.u, 0));
			VEC_GETU(R.v[1].u, 1) = (VEC_GETU(C2.u, 1) & ~VEC_GETU(g_Select0101.v.u, 1)) | (VEC_GETU(C3.u, 1) & VEC_GETU(g_Select0101.v.u, 1));
			VEC_GETU(R.v[1].u, 2) = (VEC_GETU(C2.u, 2) & ~VEC_GETU(g_Select0101.v.u, 2)) | (VEC_GETU(C3.u, 2) & VEC_GETU(g_Select0101.v.u, 2));
			VEC_GETU(R.v[1].u, 3) = (VEC_GETU(C2.u, 3) & ~VEC_GETU(g_Select0101.v.u, 3)) | (VEC_GETU(C3.u, 3) & VEC_GETU(g_Select0101.v.u, 3));

			VEC_GETU(R.v[2].u, 0) = (VEC_GETU(C4.u, 0) & ~VEC_GETU(g_Select0101.v.u, 0)) | (VEC_GETU(C5.u, 0) & VEC_GETU(g_Select0101.v.u, 0));
			VEC_GETU(R.v[2].u, 1) = (VEC_GETU(C4.u, 1) & ~VEC_GETU(g_Select0101.v.u, 1)) | (VEC_GETU(C5.u, 1) & VEC_GETU(g_Select0101.v.u, 1));
			VEC_GETU(R.v[2].u, 2) = (VEC_GETU(C4.u, 2) & ~VEC_GETU(g_Select0101.v.u, 2)) | (VEC_GETU(C5.u, 2) & VEC_GETU(g_Select0101.v.u, 2));
			VEC_GETU(R.v[2].u, 3) = (VEC_GETU(C4.u, 3) & ~VEC_GETU(g_Select0101.v.u, 3)) | (VEC_GETU(C5.u, 3) & VEC_GETU(g_Select0101.v.u, 3));

			VEC_GETU(R.v[3].u, 0) = (VEC_GETU(C6.u, 0) & ~VEC_GETU(g_Select0101.v.u, 0)) | (VEC_GETU(C7.u, 0) & VEC_GETU(g_Select0101.v.u, 0));
			VEC_GETU(R.v[3].u, 1) = (VEC_GETU(C6.u, 1) & ~VEC_GETU(g_Select0101.v.u, 1)) | (VEC_GETU(C7.u, 1) & VEC_GETU(g_Select0101.v.u, 1));
			VEC_GETU(R.v[3].u, 2) = (VEC_GETU(C6.u, 2) & ~VEC_GETU(g_Select0101.v.u, 2)) | (VEC_GETU(C7.u, 2) & VEC_GETU(g_Select0101.v.u, 2));
			VEC_GETU(R.v[3].u, 3) = (VEC_GETU(C6.u, 3) & ~VEC_GETU(g_Select0101.v.u, 3)) | (VEC_GETU(C7.u, 3) & VEC_GETU(g_Select0101.v.u, 3));

			vec Determinant = DotVec<4>(R.v[0], MT.v[0]);

			vec Reciprocal;
			VEC_GETF(Reciprocal.f, 0) = 1.f / VEC_GETF(Determinant.f, 0);
			VEC_GETF(Reciprocal.f, 1) = 1.f / VEC_GETF(Determinant.f, 1);
			VEC_GETF(Reciprocal.f, 2) = 1.f / VEC_GETF(Determinant.f, 2);
			VEC_GETF(Reciprocal.f, 3) = 1.f / VEC_GETF(Determinant.f, 3);

			v[0] = MultiplyVec(R.v[0], Reciprocal);
			v[1] = MultiplyVec(R.v[1], Reciprocal);
			v[2] = MultiplyVec(R.v[2], Reciprocal);
			v[3] = MultiplyVec(R.v[3], Reciprocal);
#else
#	if defined(SIMD_SSE2)
			__m128 vTemp1 = _mm_shuffle_ps(v[0], v[1], _MM_SHUFFLE(1, 0, 1, 0));
			__m128 vTemp3 = _mm_shuffle_ps(v[0], v[1], _MM_SHUFFLE(3, 2, 3, 2));
			__m128 vTemp2 = _mm_shuffle_ps(v[2], v[3], _MM_SHUFFLE(1, 0, 1, 0));
			__m128 vTemp4 = _mm_shuffle_ps(v[2], v[3], _MM_SHUFFLE(3, 2, 3, 2));
			__m128 MT[4];
			MT[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
			MT[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
			MT[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
			MT[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));

			__m128 V00 = _mm_shuffle_ps(MT[2], MT[2], _MM_SHUFFLE(1, 1, 0, 0));
			__m128 V10 = _mm_shuffle_ps(MT[3], MT[3], _MM_SHUFFLE(3, 2, 3, 2));
			__m128 V01 = _mm_shuffle_ps(MT[0], MT[0], _MM_SHUFFLE(1, 1, 0, 0));
			__m128 V11 = _mm_shuffle_ps(MT[1], MT[1], _MM_SHUFFLE(3, 2, 3, 2));
			__m128 V02 = _mm_shuffle_ps(MT[2], MT[0], _MM_SHUFFLE(2, 0, 2, 0));
			__m128 V12 = _mm_shuffle_ps(MT[3], MT[1], _MM_SHUFFLE(3, 1, 3, 1));

			__m128 D0 = _mm_mul_ps(V00, V10);
			__m128 D1 = _mm_mul_ps(V01, V11);
			__m128 D2 = _mm_mul_ps(V02, V12);

			V00 = _mm_shuffle_ps(MT[2], MT[2], _MM_SHUFFLE(3, 2, 3, 2));
			V10 = _mm_shuffle_ps(MT[3], MT[3], _MM_SHUFFLE(1, 1, 0, 0));
			V01 = _mm_shuffle_ps(MT[0], MT[0], _MM_SHUFFLE(3, 2, 3, 2));
			V11 = _mm_shuffle_ps(MT[1], MT[1], _MM_SHUFFLE(1, 1, 0, 0));
			V02 = _mm_shuffle_ps(MT[2], MT[0], _MM_SHUFFLE(3, 1, 3, 1));
			V12 = _mm_shuffle_ps(MT[3], MT[1], _MM_SHUFFLE(2, 0, 2, 0));

			V00 = _mm_mul_ps(V00, V10);
			V01 = _mm_mul_ps(V01, V11);
			V02 = _mm_mul_ps(V02, V12);
			D0 = _mm_sub_ps(D0, V00);
			D1 = _mm_sub_ps(D1, V01);
			D2 = _mm_sub_ps(D2, V02);
			// V11 = D0Y,D0W,D2Y,D2Y
			V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 1, 3, 1));
			V00 = _mm_shuffle_ps(MT[1], MT[1], _MM_SHUFFLE(1, 0, 2, 1));
			V10 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(0, 3, 0, 2));
			V01 = _mm_shuffle_ps(MT[0], MT[0], _MM_SHUFFLE(0, 1, 0, 2));
			V11 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(2, 1, 2, 1));
			// V13 = D1Y,D1W,D2W,D2W
			__m128 V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 3, 3, 1));
			V02 = _mm_shuffle_ps(MT[3], MT[3], _MM_SHUFFLE(1, 0, 2, 1));
			V12 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(0, 3, 0, 2));
			__m128 V03 = _mm_shuffle_ps(MT[2], MT[2], _MM_SHUFFLE(0, 1, 0, 2));
			V13 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(2, 1, 2, 1));

			__m128 C0 = _mm_mul_ps(V00, V10);
			__m128 C2 = _mm_mul_ps(V01, V11);
			__m128 C4 = _mm_mul_ps(V02, V12);
			__m128 C6 = _mm_mul_ps(V03, V13);

			// V11 = D0X,D0Y,D2X,D2X
			V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(0, 0, 1, 0));
			V00 = _mm_shuffle_ps(MT[1], MT[1], _MM_SHUFFLE(2, 1, 3, 2));
			V10 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(2, 1, 0, 3));
			V01 = _mm_shuffle_ps(MT[0], MT[0], _MM_SHUFFLE(1, 3, 2, 3));
			V11 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(0, 2, 1, 2));
			// V13 = D1X,D1Y,D2Z,D2Z
			V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(2, 2, 1, 0));
			V02 = _mm_shuffle_ps(MT[3], MT[3], _MM_SHUFFLE(2, 1, 3, 2));
			V12 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(2, 1, 0, 3));
			V03 = _mm_shuffle_ps(MT[2], MT[2], _MM_SHUFFLE(1, 3, 2, 3));
			V13 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(0, 2, 1, 2));

			V00 = _mm_mul_ps(V00, V10);
			V01 = _mm_mul_ps(V01, V11);
			V02 = _mm_mul_ps(V02, V12);
			V03 = _mm_mul_ps(V03, V13);
			C0 = _mm_sub_ps(C0, V00);
			C2 = _mm_sub_ps(C2, V01);
			C4 = _mm_sub_ps(C4, V02);
			C6 = _mm_sub_ps(C6, V03);

			V00 = _mm_shuffle_ps(MT[1], MT[1], _MM_SHUFFLE(0, 3, 0, 3));
			// V10 = D0Z,D0Z,D2X,D2Y
			V10 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 2, 2));
			V10 = _mm_shuffle_ps(V10, V10, _MM_SHUFFLE(0, 2, 3, 0));
			V01 = _mm_shuffle_ps(MT[0], MT[0], _MM_SHUFFLE(2, 0, 3, 1));
			// V11 = D0X,D0W,D2X,D2Y
			V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 3, 0));
			V11 = _mm_shuffle_ps(V11, V11, _MM_SHUFFLE(2, 1, 0, 3));
			V02 = _mm_shuffle_ps(MT[3], MT[3], _MM_SHUFFLE(0, 3, 0, 3));
			// V12 = D1Z,D1Z,D2Z,D2W
			V12 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 2, 2));
			V12 = _mm_shuffle_ps(V12, V12, _MM_SHUFFLE(0, 2, 3, 0));
			V03 = _mm_shuffle_ps(MT[2], MT[2], _MM_SHUFFLE(2, 0, 3, 1));
			// V13 = D1X,D1W,D2Z,D2W
			V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 3, 0));
			V13 = _mm_shuffle_ps(V13, V13, _MM_SHUFFLE(2, 1, 0, 3));

			V00 = _mm_mul_ps(V00, V10);
			V01 = _mm_mul_ps(V01, V11);
			V02 = _mm_mul_ps(V02, V12);
			V03 = _mm_mul_ps(V03, V13);
			__m128 C1 = _mm_sub_ps(C0, V00);
			C0 = _mm_add_ps(C0, V00);
			__m128 C3 = _mm_add_ps(C2, V01);
			C2 = _mm_sub_ps(C2, V01);
			__m128 C5 = _mm_sub_ps(C4, V02);
			C4 = _mm_add_ps(C4, V02);
			__m128 C7 = _mm_add_ps(C6, V03);
			C6 = _mm_sub_ps(C6, V03);

			C0 = _mm_shuffle_ps(C0, C1, _MM_SHUFFLE(3, 1, 2, 0));
			C2 = _mm_shuffle_ps(C2, C3, _MM_SHUFFLE(3, 1, 2, 0));
			C4 = _mm_shuffle_ps(C4, C5, _MM_SHUFFLE(3, 1, 2, 0));
			C6 = _mm_shuffle_ps(C6, C7, _MM_SHUFFLE(3, 1, 2, 0));
			C0 = _mm_shuffle_ps(C0, C0, _MM_SHUFFLE(3, 1, 2, 0));
			C2 = _mm_shuffle_ps(C2, C2, _MM_SHUFFLE(3, 1, 2, 0));
			C4 = _mm_shuffle_ps(C4, C4, _MM_SHUFFLE(3, 1, 2, 0));
			C6 = _mm_shuffle_ps(C6, C6, _MM_SHUFFLE(3, 1, 2, 0));
			__m128 vTemp = DotVec<4>(C0, MT[0]);
			vTemp = _mm_div_ps(g_One.v, vTemp);
			__m128 mResult[4];
			mResult[0] = _mm_mul_ps(C0, vTemp);
			mResult[1] = _mm_mul_ps(C2, vTemp);
			mResult[2] = _mm_mul_ps(C4, vTemp);
			mResult[3] = _mm_mul_ps(C6, vTemp);

			memcpy(this->v, mResult, sizeof(mResult));
#	else
#	endif
#endif
		}
		
		MATH_CALL(matrix) operator*(const matrix & a, float b)
		{
#ifndef MATH_USE_SIMD
			matrix R;

			R.v[0].f[0] = a.v[0].f[0] * b;
			R.v[0].f[1] = a.v[0].f[1] * b;
			R.v[0].f[2] = a.v[0].f[2] * b;
			R.v[0].f[3] = a.v[0].f[3] * b;
			R.v[1].f[0] = a.v[1].f[0] * b;
			R.v[1].f[1] = a.v[1].f[1] * b;
			R.v[1].f[2] = a.v[1].f[2] * b;
			R.v[1].f[3] = a.v[1].f[3] * b;
			R.v[2].f[0] = a.v[2].f[0] * b;
			R.v[2].f[1] = a.v[2].f[1] * b;
			R.v[2].f[2] = a.v[2].f[2] * b;
			R.v[2].f[3] = a.v[2].f[3] * b;
			R.v[3].f[0] = a.v[3].f[0] * b;
			R.v[3].f[1] = a.v[3].f[1] * b;
			R.v[3].f[2] = a.v[3].f[2] * b;
			R.v[3].f[3] = a.v[3].f[3] * b;
			return R;
#else
#	if defined(SIMD_SSE2)
			matrix R;
			__m128 vResult = _mm_set_ps1(b);
			R.v[0] = _mm_mul_ps(vResult, a.v[0]);
			R.v[1] = _mm_mul_ps(vResult, a.v[1]);
			R.v[2] = _mm_mul_ps(vResult, a.v[2]);
			R.v[3] = _mm_mul_ps(vResult, a.v[3]);
			return R;
#	else
			matrix R;
			R.v[0] = vmulq_n_f32(a.v[0].f, b);
			R.v[1] = vmulq_n_f32(a.v[1].f, b);
			R.v[2] = vmulq_n_f32(a.v[2].f, b);
			R.v[3] = vmulq_n_f32(a.v[3].f, b);
			return R;
#	endif
#endif
		}

		MATH_CALL(matrix&) matrix::operator/=(float a)
		{
#ifndef MATH_USE_SIMD
			v[0].f[0] = v[0].f[0] / a;
			v[0].f[1] = v[0].f[1] / a;
			v[0].f[2] = v[0].f[2] / a;
			v[0].f[3] = v[0].f[3] / a;
			v[1].f[0] = v[1].f[0] / a;
			v[1].f[1] = v[1].f[1] / a;
			v[1].f[2] = v[1].f[2] / a;
			v[1].f[3] = v[1].f[3] / a;
			v[2].f[0] = v[2].f[0] / a;
			v[2].f[1] = v[2].f[1] / a;
			v[2].f[2] = v[2].f[2] / a;
			v[2].f[3] = v[2].f[3] / a;
			v[3].f[0] = v[3].f[0] / a;
			v[3].f[1] = v[3].f[1] / a;
			v[3].f[2] = v[3].f[2] / a;
			v[3].f[3] = v[3].f[3] / a;
			return *this;
#else
#	if defined(SIMD_SSE2)
			__m128 vS = _mm_set_ps1(a);
			v[0] = _mm_div_ps(v[0], vS);
			v[1] = _mm_div_ps(v[1], vS);
			v[2] = _mm_div_ps(v[2], vS);
			v[3] = _mm_div_ps(v[3], vS);
			return *this;
#	else
			float32x2_t vS = vdup_n_f32(a);
			float32x2_t R0 = vrecpe_f32(vS);
			float32x2_t S0 = vrecps_f32(R0, vS);
			R0 = vmul_f32(S0, R0);
			S0 = vrecps_f32(R0, vS);
			R0 = vmul_f32(S0, R0);
			float32x4_t Reciprocal = vec(vcombine_u32(hvec(R0).u, hvec(R0).u)).f;
			v[0] = vmulq_f32(v[0].f, Reciprocal);
			v[1] = vmulq_f32(v[1].f, Reciprocal);
			v[2] = vmulq_f32(v[2].f, Reciprocal);
			v[3] = vmulq_f32(v[3].f, Reciprocal);
			return *this;
#	endif
#endif
		}
		
		MATH_CALL(matrix) operator*(const matrix & a, const matrix & b)
		{
#ifndef MATH_USE_SIMD
			matrix mResult;
			// Cache the invariants in registers
			float x = a.v[0].f[0];
			float y = a.v[0].f[1];
			float z = a.v[0].f[2];
			float w = a.v[0].f[3];
			// Perform the operation on the first row
			mResult.v[0].f[0] = (b.v[0].f[0] * x) + (b.v[1].f[0] * y) + (b.v[2].f[0] * z) + (b.v[3].f[0] * w);
			mResult.v[0].f[1] = (b.v[0].f[1] * x) + (b.v[1].f[1] * y) + (b.v[2].f[1] * z) + (b.v[3].f[1] * w);
			mResult.v[0].f[2] = (b.v[0].f[2] * x) + (b.v[1].f[2] * y) + (b.v[2].f[2] * z) + (b.v[3].f[2] * w);
			mResult.v[0].f[3] = (b.v[0].f[3] * x) + (b.v[1].f[3] * y) + (b.v[2].f[3] * z) + (b.v[3].f[3] * w);
			// Repeat for all the other rows
			x = a.v[1].f[0];
			y = a.v[1].f[1];
			z = a.v[1].f[2];
			w = a.v[1].f[3];
			mResult.v[1].f[0] = (b.v[0].f[0] * x) + (b.v[1].f[0] * y) + (b.v[2].f[0] * z) + (b.v[3].f[0] * w);
			mResult.v[1].f[1] = (b.v[0].f[1] * x) + (b.v[1].f[1] * y) + (b.v[2].f[1] * z) + (b.v[3].f[1] * w);
			mResult.v[1].f[2] = (b.v[0].f[2] * x) + (b.v[1].f[2] * y) + (b.v[2].f[2] * z) + (b.v[3].f[2] * w);
			mResult.v[1].f[3] = (b.v[0].f[3] * x) + (b.v[1].f[3] * y) + (b.v[2].f[3] * z) + (b.v[3].f[3] * w);
			x = a.v[2].f[0];
			y = a.v[2].f[1];
			z = a.v[2].f[2];
			w = a.v[2].f[3];
			mResult.v[2].f[0] = (b.v[0].f[0] * x) + (b.v[1].f[0] * y) + (b.v[2].f[0] * z) + (b.v[3].f[0] * w);
			mResult.v[2].f[1] = (b.v[0].f[1] * x) + (b.v[1].f[1] * y) + (b.v[2].f[1] * z) + (b.v[3].f[1] * w);
			mResult.v[2].f[2] = (b.v[0].f[2] * x) + (b.v[1].f[2] * y) + (b.v[2].f[2] * z) + (b.v[3].f[2] * w);
			mResult.v[2].f[3] = (b.v[0].f[3] * x) + (b.v[1].f[3] * y) + (b.v[2].f[3] * z) + (b.v[3].f[3] * w);
			x = a.v[3].f[0];
			y = a.v[3].f[1];
			z = a.v[3].f[2];
			w = a.v[3].f[3];
			mResult.v[3].f[0] = (b.v[0].f[0] * x) + (b.v[1].f[0] * y) + (b.v[2].f[0] * z) + (b.v[3].f[0] * w);
			mResult.v[3].f[1] = (b.v[0].f[1] * x) + (b.v[1].f[1] * y) + (b.v[2].f[1] * z) + (b.v[3].f[1] * w);
			mResult.v[3].f[2] = (b.v[0].f[2] * x) + (b.v[1].f[2] * y) + (b.v[2].f[2] * z) + (b.v[3].f[2] * w);
			mResult.v[3].f[3] = (b.v[0].f[3] * x) + (b.v[1].f[3] * y) + (b.v[2].f[3] * z) + (b.v[3].f[3] * w);
			return mResult;
#else
#	if defined(SIMD_SSE2)
			matrix mResult;
			// Use vW to hold the original row
			__m128 vW = a.v[0];
			// Splat the component X,Y,Z then W
			__m128 vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
			__m128 vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
			vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));
			// Perform the operation on the first row
			vX = _mm_mul_ps(vX, b.v[0]);
			vY = _mm_mul_ps(vY, b.v[1]);
			vZ = _mm_mul_ps(vZ, b.v[2]);
			vW = _mm_mul_ps(vW, b.v[3]);
			// Perform a binary add to reduce cumulative errors
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			mResult.v[0] = vX;
			// Repeat for the other 3 rows
			vW = a.v[1];
			vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
			vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
			vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
			vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));
			vX = _mm_mul_ps(vX, b.v[0]);
			vY = _mm_mul_ps(vY, b.v[1]);
			vZ = _mm_mul_ps(vZ, b.v[2]);
			vW = _mm_mul_ps(vW, b.v[3]);
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			mResult.v[1] = vX;
			vW = a.v[2];
			vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
			vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
			vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
			vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));
			vX = _mm_mul_ps(vX, b.v[0]);
			vY = _mm_mul_ps(vY, b.v[1]);
			vZ = _mm_mul_ps(vZ, b.v[2]);
			vW = _mm_mul_ps(vW, b.v[3]);
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			mResult.v[2] = vX;
			vW = a.v[3];
			vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
			vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
			vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
			vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));
			vX = _mm_mul_ps(vX, b.v[0]);
			vY = _mm_mul_ps(vY, b.v[1]);
			vZ = _mm_mul_ps(vZ, b.v[2]);
			vW = _mm_mul_ps(vW, b.v[3]);
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			mResult.v[3] = vX;
			return mResult;
#	else
			matrix mResult;
			float32x2_t VL = vget_low_f32(a.v[0].f);
			float32x2_t VH = vget_high_f32(a.v[0].f);
			// Perform the operation on the first row
			vec vX = vmulq_lane_f32(b.v[0].f, VL, 0);
			vec vY = vmulq_lane_f32(b.v[1].f, VL, 1);
			vec vZ = vmlaq_lane_f32(vX.f, b.v[2].f, VH, 0);
			vec vW = vmlaq_lane_f32(vY.f, b.v[3].f, VH, 1);
			mResult.v[0] = vaddq_f32(vZ.f, vW.f);
			// Repeat for the other 3 rows
			VL = vget_low_f32(a.v[1].f);
			VH = vget_high_f32(a.v[1].f);
			vX = vmulq_lane_f32(b.v[0].f, VL, 0);
			vY = vmulq_lane_f32(b.v[1].f, VL, 1);
			vZ = vmlaq_lane_f32(vX.f, b.v[2].f, VH, 0);
			vW = vmlaq_lane_f32(vY.f, b.v[3].f, VH, 1);
			mResult.v[1] = vaddq_f32(vZ.f, vW.f);
			VL = vget_low_f32(a.v[2].f);
			VH = vget_high_f32(a.v[2].f);
			vX = vmulq_lane_f32(b.v[0].f, VL, 0);
			vY = vmulq_lane_f32(b.v[1].f, VL, 1);
			vZ = vmlaq_lane_f32(vX.f, b.v[2].f, VH, 0);
			vW = vmlaq_lane_f32(vY.f, b.v[3].f, VH, 1);
			mResult.v[2] = vaddq_f32(vZ.f, vW.f);
			VL = vget_low_f32(a.v[3].f);
			VH = vget_high_f32(a.v[3].f);
			vX = vmulq_lane_f32(b.v[0].f, VL, 0);
			vY = vmulq_lane_f32(b.v[1].f, VL, 1);
			vZ = vmlaq_lane_f32(vX.f, b.v[2].f, VH, 0);
			vW = vmlaq_lane_f32(vY.f, b.v[3].f, VH, 1);
			mResult.v[3] = vaddq_f32(vZ.f, vW.f);
			return mResult;
#	endif
#endif
		}
		
		MATH_CALL(matrix) operator/(const matrix & a, float b)
		{
#ifndef MATH_USE_SIMD
			matrix R;

			R.v[0].f[0] = a.v[0].f[0] / b;
			R.v[0].f[1] = a.v[0].f[1] / b;
			R.v[0].f[2] = a.v[0].f[2] / b;
			R.v[0].f[3] = a.v[0].f[3] / b;
			R.v[1].f[0] = a.v[1].f[0] / b;
			R.v[1].f[1] = a.v[1].f[1] / b;
			R.v[1].f[2] = a.v[1].f[2] / b;
			R.v[1].f[3] = a.v[1].f[3] / b;
			R.v[2].f[0] = a.v[2].f[0] / b;
			R.v[2].f[1] = a.v[2].f[1] / b;
			R.v[2].f[2] = a.v[2].f[2] / b;
			R.v[2].f[3] = a.v[2].f[3] / b;
			R.v[3].f[0] = a.v[3].f[0] / b;
			R.v[3].f[1] = a.v[3].f[1] / b;
			R.v[3].f[2] = a.v[3].f[2] / b;
			R.v[3].f[3] = a.v[3].f[3] / b;
			return R;
#else
#	if defined(SIMD_SSE2)
			matrix R;
			__m128 vResult = _mm_set_ps1(b);
			R.v[0] = _mm_div_ps(vResult, a.v[0]);
			R.v[1] = _mm_div_ps(vResult, a.v[1]);
			R.v[2] = _mm_div_ps(vResult, a.v[2]);
			R.v[3] = _mm_div_ps(vResult, a.v[3]);
			return R;
#	else
			float32x2_t vS = vdup_n_f32(b);
			float32x2_t R0 = vrecpe_f32(vS);
			float32x2_t S0 = vrecps_f32(R0, vS);
			R0 = vmul_f32(S0, R0);
			S0 = vrecps_f32(R0, vS);
			R0 = vmul_f32(S0, R0);
			float32x4_t Reciprocal = vec(vcombine_u32(hvec(R0).u, hvec(R0).u)).f;
			matrix R;
			R.v[0] = vmulq_f32(a.v[0].f, Reciprocal);
			R.v[1] = vmulq_f32(a.v[1].f, Reciprocal);
			R.v[2] = vmulq_f32(a.v[2].f, Reciprocal);
			R.v[3] = vmulq_f32(a.v[3].f, Reciprocal);
			return R;
#	endif
#endif
		}
		
		MATH_CALL(matrix&) matrix::operator*=(float a)
		{
#ifndef MATH_USE_SIMD
			v[0].f[0] = v[0].f[0] * a;
			v[0].f[1] = v[0].f[1] * a;
			v[0].f[2] = v[0].f[2] * a;
			v[0].f[3] = v[0].f[3] * a;
			v[1].f[0] = v[1].f[0] * a;
			v[1].f[1] = v[1].f[1] * a;
			v[1].f[2] = v[1].f[2] * a;
			v[1].f[3] = v[1].f[3] * a;
			v[2].f[0] = v[2].f[0] * a;
			v[2].f[1] = v[2].f[1] * a;
			v[2].f[2] = v[2].f[2] * a;
			v[2].f[3] = v[2].f[3] * a;
			v[3].f[0] = v[3].f[0] * a;
			v[3].f[1] = v[3].f[1] * a;
			v[3].f[2] = v[3].f[2] * a;
			v[3].f[3] = v[3].f[3] * a;
			return *this;
#else
#	if defined(SIMD_SSE2)
			__m128 vS = _mm_set_ps1(a);
			v[0] = _mm_mul_ps(v[0], vS);
			v[1] = _mm_mul_ps(v[1], vS);
			v[2] = _mm_mul_ps(v[2], vS);
			v[3] = _mm_mul_ps(v[3], vS);
			return *this;
#	else
			v[0] = vmulq_n_f32(v[0].f, a);
			v[1] = vmulq_n_f32(v[1].f, a);
			v[2] = vmulq_n_f32(v[2].f, a);
			v[3] = vmulq_n_f32(v[3].f, a);
			return *this;
#	endif
#endif
		}
		
		MATH_CALL(matrix&) matrix::operator*=(const matrix & a)
		{
			*this = *this * a;
			return *this;
		}
		
		MATH_CALL(matrix) operator-(const matrix & a, const matrix & b)
		{
#ifndef MATH_USE_SIMD
			matrix Result;
			Result.v[0].f[0] = a.v[0].f[0] - b.v[0].f[0];
			Result.v[0].f[1] = a.v[0].f[1] - b.v[0].f[1];
			Result.v[0].f[2] = a.v[0].f[2] - b.v[0].f[2];
			Result.v[0].f[3] = a.v[0].f[3] - b.v[0].f[3];
			Result.v[1].f[0] = a.v[1].f[0] - b.v[1].f[0];
			Result.v[1].f[1] = a.v[1].f[1] - b.v[1].f[1];
			Result.v[1].f[2] = a.v[1].f[2] - b.v[1].f[2];
			Result.v[1].f[3] = a.v[1].f[3] - b.v[1].f[3];
			Result.v[2].f[0] = a.v[2].f[0] - b.v[2].f[0];
			Result.v[2].f[1] = a.v[2].f[1] - b.v[2].f[1];
			Result.v[2].f[2] = a.v[2].f[2] - b.v[2].f[2];
			Result.v[2].f[3] = a.v[2].f[3] - b.v[2].f[3];
			Result.v[3].f[0] = a.v[3].f[0] - b.v[3].f[0];
			Result.v[3].f[1] = a.v[3].f[1] - b.v[3].f[1];
			Result.v[3].f[2] = a.v[3].f[2] - b.v[3].f[2];
			Result.v[3].f[3] = a.v[3].f[3] - b.v[3].f[3];
			return Result;
#else
#	if defined(SIMD_SSE2)
			matrix R;
			R.v[0] = _mm_sub_ps(a.v[0], b.v[0]);
			R.v[1] = _mm_sub_ps(a.v[1], b.v[1]);
			R.v[2] = _mm_sub_ps(a.v[2], b.v[2]);
			R.v[3] = _mm_sub_ps(a.v[3], b.v[3]);
			return R;
#	else
			matrix R;
			R.v[0] = vsubq_f32(a.v[0].f, b.v[0].f);
			R.v[1] = vsubq_f32(a.v[1].f, b.v[1].f);
			R.v[2] = vsubq_f32(a.v[2].f, b.v[2].f);
			R.v[3] = vsubq_f32(a.v[3].f, b.v[3].f);
			return R;
#	endif
#endif
		}
		
		MATH_CALL(matrix) operator+(const matrix & a, const matrix & b)
		{
#ifndef MATH_USE_SIMD
			matrix Result;
			Result.v[0].f[0] = a.v[0].f[0] + b.v[0].f[0];
			Result.v[0].f[1] = a.v[0].f[1] + b.v[0].f[1];
			Result.v[0].f[2] = a.v[0].f[2] + b.v[0].f[2];
			Result.v[0].f[3] = a.v[0].f[3] + b.v[0].f[3];
			Result.v[1].f[0] = a.v[1].f[0] + b.v[1].f[0];
			Result.v[1].f[1] = a.v[1].f[1] + b.v[1].f[1];
			Result.v[1].f[2] = a.v[1].f[2] + b.v[1].f[2];
			Result.v[1].f[3] = a.v[1].f[3] + b.v[1].f[3];
			Result.v[2].f[0] = a.v[2].f[0] + b.v[2].f[0];
			Result.v[2].f[1] = a.v[2].f[1] + b.v[2].f[1];
			Result.v[2].f[2] = a.v[2].f[2] + b.v[2].f[2];
			Result.v[2].f[3] = a.v[2].f[3] + b.v[2].f[3];
			Result.v[3].f[0] = a.v[3].f[0] + b.v[3].f[0];
			Result.v[3].f[1] = a.v[3].f[1] + b.v[3].f[1];
			Result.v[3].f[2] = a.v[3].f[2] + b.v[3].f[2];
			Result.v[3].f[3] = a.v[3].f[3] + b.v[3].f[3];
			return Result;
#else
#	if defined(SIMD_SSE2)
			matrix R;
			R.v[0] = _mm_add_ps(a.v[0], b.v[0]);
			R.v[1] = _mm_add_ps(a.v[1], b.v[1]);
			R.v[2] = _mm_add_ps(a.v[2], b.v[2]);
			R.v[3] = _mm_add_ps(a.v[3], b.v[3]);
			return R;
#	else
			matrix R;
			R.v[0] = vaddq_f32(a.v[0].f, b.v[0].f);
			R.v[1] = vaddq_f32(a.v[1].f, b.v[1].f);
			R.v[2] = vaddq_f32(a.v[2].f, b.v[2].f);
			R.v[3] = vaddq_f32(a.v[3].f, b.v[3].f);
			return R;
#	endif
#endif
		}
		
		MATH_CALL(matrix&) matrix::operator+=(const matrix & a)
		{
#ifndef MATH_USE_SIMD
			v[0].f[0] = v[0].f[0] + a.v[0].f[0];
			v[0].f[1] = v[0].f[1] + a.v[0].f[1];
			v[0].f[2] = v[0].f[2] + a.v[0].f[2];
			v[0].f[3] = v[0].f[3] + a.v[0].f[3];
			v[1].f[0] = v[1].f[0] + a.v[1].f[0];
			v[1].f[1] = v[1].f[1] + a.v[1].f[1];
			v[1].f[2] = v[1].f[2] + a.v[1].f[2];
			v[1].f[3] = v[1].f[3] + a.v[1].f[3];
			v[2].f[0] = v[2].f[0] + a.v[2].f[0];
			v[2].f[1] = v[2].f[1] + a.v[2].f[1];
			v[2].f[2] = v[2].f[2] + a.v[2].f[2];
			v[2].f[3] = v[2].f[3] + a.v[2].f[3];
			v[3].f[0] = v[3].f[0] + a.v[3].f[0];
			v[3].f[1] = v[3].f[1] + a.v[3].f[1];
			v[3].f[2] = v[3].f[2] + a.v[3].f[2];
			v[3].f[3] = v[3].f[3] + a.v[3].f[3];
			return *this;
#else
#	if defined(SIMD_SSE2)
			this->v[0] = _mm_add_ps(v[0], a.v[0]);
			this->v[1] = _mm_add_ps(v[1], a.v[1]);
			this->v[2] = _mm_add_ps(v[2], a.v[2]);
			this->v[3] = _mm_add_ps(v[3], a.v[3]);
			return *this;
#	else
			v[0] = vaddq_f32(v[0].f, a.v[0].f);
			v[1] = vaddq_f32(v[1].f, a.v[1].f);
			v[2] = vaddq_f32(v[2].f, a.v[2].f);
			v[3] = vaddq_f32(v[3].f, a.v[3].f);
			return *this;
#	endif
#endif
		}

		MATH_CALL(matrix&) matrix::operator-=(const matrix & a)
		{
#ifndef MATH_USE_SIMD
			v[0].f[0] = v[0].f[0] - a.v[0].f[0];
			v[0].f[1] = v[0].f[1] - a.v[0].f[1];
			v[0].f[2] = v[0].f[2] - a.v[0].f[2];
			v[0].f[3] = v[0].f[3] - a.v[0].f[3];
			v[1].f[0] = v[1].f[0] - a.v[1].f[0];
			v[1].f[1] = v[1].f[1] - a.v[1].f[1];
			v[1].f[2] = v[1].f[2] - a.v[1].f[2];
			v[1].f[3] = v[1].f[3] - a.v[1].f[3];
			v[2].f[0] = v[2].f[0] - a.v[2].f[0];
			v[2].f[1] = v[2].f[1] - a.v[2].f[1];
			v[2].f[2] = v[2].f[2] - a.v[2].f[2];
			v[2].f[3] = v[2].f[3] - a.v[2].f[3];
			v[3].f[0] = v[3].f[0] - a.v[3].f[0];
			v[3].f[1] = v[3].f[1] - a.v[3].f[1];
			v[3].f[2] = v[3].f[2] - a.v[3].f[2];
			v[3].f[3] = v[3].f[3] - a.v[3].f[3];
			return *this;
#else
#	if defined(SIMD_SSE2)
			this->v[0] = _mm_sub_ps(v[0], a.v[0]);
			this->v[1] = _mm_sub_ps(v[1], a.v[1]);
			this->v[2] = _mm_sub_ps(v[2], a.v[2]);
			this->v[3] = _mm_sub_ps(v[3], a.v[3]);
			return *this;
#	else
			v[0] = vsubq_f32(v[0].f, a.v[0].f);
			v[1] = vsubq_f32(v[1].f, a.v[1].f);
			v[2] = vsubq_f32(v[2].f, a.v[2].f);
			v[3] = vsubq_f32(v[3].f, a.v[3].f);
			return *this;
#	endif
#endif
		}

		MATH_CALL(matrix) matrix::Identity()
		{
			matrix r;
			r.v[0] = g_IdentityR0.v;
			r.v[1] = g_IdentityR1.v;
			r.v[2] = g_IdentityR2.v;
			r.v[3] = g_IdentityR3.v;
			return r;
		}
		
		MATH_CALL(matrix) matrix::RotationX(float x)
		{
#ifndef MATH_USE_SIMD
			float    fSinAngle;
			float    fCosAngle;
			sin_cos(x, &fSinAngle, &fCosAngle);

			matrix M;
			M.v[0].f[0] = 1.0f;
			M.v[0].f[1] = 0.0f;
			M.v[0].f[2] = 0.0f;
			M.v[0].f[3] = 0.0f;

			M.v[1].f[0] = 0.0f;
			M.v[1].f[1] = fCosAngle;
			M.v[1].f[2] = fSinAngle;
			M.v[1].f[3] = 0.0f;

			M.v[2].f[0] = 0.0f;
			M.v[2].f[1] = -fSinAngle;
			M.v[2].f[2] = fCosAngle;
			M.v[2].f[3] = 0.0f;

			M.v[3].f[0] = 0.0f;
			M.v[3].f[1] = 0.0f;
			M.v[3].f[2] = 0.0f;
			M.v[3].f[3] = 1.0f;
			return M;
#else
#	if defined(SIMD_SSE2)
			float    SinAngle;
			float    CosAngle;
			sin_cos(x, &SinAngle, &CosAngle);

			__m128 vSin = _mm_set_ss(SinAngle);
			__m128 vCos = _mm_set_ss(CosAngle);
			// x = 0,y = cos,z = sin, w = 0
			vCos = _mm_shuffle_ps(vCos, vSin, _MM_SHUFFLE(3, 0, 0, 3));
			matrix M;
			M.v[0] = g_IdentityR0.v;
			M.v[1] = vCos;
			// x = 0,y = sin,z = cos, w = 0
			vCos = _mm_shuffle_ps(vCos, vCos, _MM_SHUFFLE(3, 1, 2, 0));
			// x = 0,y = -sin,z = cos, w = 0
			vCos = _mm_mul_ps(vCos, g_NegateY.v);
			M.v[2] = vCos;
			M.v[3] = g_IdentityR3.v;
			return M;
#	else
			float    fSinAngle;
			float    fCosAngle;
			sin_cos(x, &fSinAngle, &fCosAngle);

			const vec Zero = vdupq_n_f32(0);

			vec T1 = vsetq_lane_f32(fCosAngle, Zero.f, 1);
			T1 = vsetq_lane_f32(fSinAngle, T1.f, 2);

			vec T2 = vsetq_lane_f32(-fSinAngle, Zero.f, 1);
			T2 = vsetq_lane_f32(fCosAngle, T2.f, 2);

			matrix M;
			M.v[0] = g_IdentityR0.v;
			M.v[1] = T1;
			M.v[2] = T2;
			M.v[3] = g_IdentityR3.v;
			return M;
#	endif
#endif
		}

		MATH_CALL(matrix) matrix::RotationY(float y)
		{
#ifndef MATH_USE_SIMD
			float    fSinAngle;
			float    fCosAngle;
			sin_cos(y, &fSinAngle, &fCosAngle);

			matrix M;
			M.v[0].f[0] = fCosAngle;
			M.v[0].f[1] = 0.0f;
			M.v[0].f[2] = -fSinAngle;
			M.v[0].f[3] = 0.0f;

			M.v[1].f[0] = 0.0f;
			M.v[1].f[1] = 1.0f;
			M.v[1].f[2] = 0.0f;
			M.v[1].f[3] = 0.0f;

			M.v[2].f[0] = fSinAngle;
			M.v[2].f[1] = 0.0f;
			M.v[2].f[2] = fCosAngle;
			M.v[2].f[3] = 0.0f;

			M.v[3].f[0] = 0.0f;
			M.v[3].f[1] = 0.0f;
			M.v[3].f[2] = 0.0f;
			M.v[3].f[3] = 1.0f;
			return M;
#else
#	if defined(SIMD_SSE2)
			float    SinAngle;
			float    CosAngle;
			sin_cos(y, &SinAngle, &CosAngle);

			__m128 vSin = _mm_set_ss(SinAngle);
			__m128 vCos = _mm_set_ss(CosAngle);
			// x = sin,y = 0,z = cos, w = 0
			vSin = _mm_shuffle_ps(vSin, vCos, _MM_SHUFFLE(3, 0, 3, 0));
			matrix M;
			M.v[2] = vSin;
			M.v[1] = g_IdentityR1.v;
			// x = cos,y = 0,z = sin, w = 0
			vSin = _mm_shuffle_ps(vSin, vSin, _MM_SHUFFLE(3, 0, 1, 2));
			// x = cos,y = 0,z = -sin, w = 0
			vSin = _mm_mul_ps(vSin, g_NegateZ.v);
			M.v[0] = vSin;
			M.v[3] = g_IdentityR3.v;
			return M;
#	else
			float    fSinAngle;
			float    fCosAngle;
			sin_cos(y, &fSinAngle, &fCosAngle);

			const vec Zero = vdupq_n_f32(0);

			vec T0 = vsetq_lane_f32(fCosAngle, Zero.f, 0);
			T0 = vsetq_lane_f32(-fSinAngle, T0.f, 2);

			vec T2 = vsetq_lane_f32(fSinAngle, Zero.f, 0);
			T2 = vsetq_lane_f32(fCosAngle, T2.f, 2);

			matrix M;
			M.v[0] = T0;
			M.v[1] = g_IdentityR1.v;
			M.v[2] = T2;
			M.v[3] = g_IdentityR3.v;
			return M;
#	endif
#endif
		}

		MATH_CALL(matrix) matrix::RotationZ(float z)
		{
#ifndef MATH_USE_SIMD
			float    fSinAngle;
			float    fCosAngle;
			sin_cos(z, &fSinAngle, &fCosAngle);

			matrix M;
			M.v[0].f[0] = fCosAngle;
			M.v[0].f[1] = fSinAngle;
			M.v[0].f[2] = 0.0f;
			M.v[0].f[3] = 0.0f;

			M.v[1].f[0] = -fSinAngle;
			M.v[1].f[1] = fCosAngle;
			M.v[1].f[2] = 0.0f;
			M.v[1].f[3] = 0.0f;

			M.v[2].f[0] = 0.0f;
			M.v[2].f[1] = 0.0f;
			M.v[2].f[2] = 1.0f;
			M.v[2].f[3] = 0.0f;

			M.v[3].f[0] = 0.0f;
			M.v[3].f[1] = 0.0f;
			M.v[3].f[2] = 0.0f;
			M.v[3].f[3] = 1.0f;
			return M;
#else
#	if defined(SIMD_SSE2)
			float    SinAngle;
			float    CosAngle;
			sin_cos(z, &SinAngle, &CosAngle);

			__m128 vSin = _mm_set_ss(SinAngle);
			__m128 vCos = _mm_set_ss(CosAngle);
			// x = cos,y = sin,z = 0, w = 0
			vCos = _mm_unpacklo_ps(vCos, vSin);
			matrix M;
			M.v[0] = vCos;
			// x = sin,y = cos,z = 0, w = 0
			vCos = _mm_shuffle_ps(vCos, vCos, _MM_SHUFFLE(3, 2, 0, 1));
			// x = cos,y = -sin,z = 0, w = 0
			vCos = _mm_mul_ps(vCos, g_NegateX.v);
			M.v[1] = vCos;
			M.v[2] = g_IdentityR2.v;
			M.v[3] = g_IdentityR3.v;
			return M;
#	else
			float    fSinAngle;
			float    fCosAngle;
			sin_cos(z, &fSinAngle, &fCosAngle);

			const vec Zero = vdupq_n_f32(0);

			vec T0 = vsetq_lane_f32(fCosAngle, Zero.f, 0);
			T0 = vsetq_lane_f32(fSinAngle, T0.f, 1);

			vec T1 = vsetq_lane_f32(-fSinAngle, Zero.f, 0);
			T1 = vsetq_lane_f32(fCosAngle, T1.f, 1);

			matrix M;
			M.v[0] = T0;
			M.v[1] = T1;
			M.v[2] = g_IdentityR2.v;
			M.v[3] = g_IdentityR3.v;
			return M;
#	endif
#endif
		}
		
		MATH_CALL(matrix) matrix::RotationAxis(const float4& axis_, float angle)
		{
#ifndef MATH_USE_SIMD
			assert(!(axis_.x == 0.f && axis_.y == 0.f && axis_.z == 0.f));
			assert(!(ISINF(axis_.x) || ISINF(axis_.y) || ISINF(axis_.z)));

			vec NormalAxis = NormalizeVec<3>(LoadVec<3>(reinterpret_cast<const float*>(&axis_)));

			float    fSinAngle;
			float    fCosAngle;
			sin_cos(angle, &fSinAngle, &fCosAngle);

			vec A = { fSinAngle, fCosAngle, 1.0f - fCosAngle, 0.0f };

			vec C2 = { A.f[2], A.f[2], A.f[2], A.f[2] };
			vec C1 = { A.f[1], A.f[1], A.f[1], A.f[1] };
			vec C0 = { A.f[0], A.f[0], A.f[0], A.f[0] };

			vec N0 = { NormalAxis.f[1], NormalAxis.f[2], NormalAxis.f[0], NormalAxis.f[3] };
			vec N1 = { NormalAxis.f[2], NormalAxis.f[0], NormalAxis.f[1], NormalAxis.f[3] };

			vec V0 = MultiplyVec(C2, N0);
			V0 = MultiplyVec(V0, N1);

			vec R0 = MultiplyVec(C2, NormalAxis);
			R0 = AddVec(MultiplyVec(R0, NormalAxis), C1);

			vec R1 = AddVec(MultiplyVec(C0, NormalAxis), V0);

			vec R2;
			R2.f[0] = V0.f[0] - (C0.f[0] * NormalAxis.f[0]);
			R2.f[1] = V0.f[1] - (C0.f[1] * NormalAxis.f[1]);
			R2.f[2] = V0.f[2] - (C0.f[2] * NormalAxis.f[2]);
			R2.f[3] = V0.f[3] - (C0.f[3] * NormalAxis.f[3]);

			V0.u[0] = (A.u[0] & ~g_Select1110.v.i[0]) | (R0.u[0] & g_Select1110.v.i[0]);
			V0.u[1] = (A.u[1] & ~g_Select1110.v.i[1]) | (R0.u[1] & g_Select1110.v.i[1]);
			V0.u[2] = (A.u[2] & ~g_Select1110.v.i[2]) | (R0.u[2] & g_Select1110.v.i[2]);
			V0.u[3] = (A.u[3] & ~g_Select1110.v.i[3]) | (R0.u[3] & g_Select1110.v.i[3]);

			vec V1 = Permute<2, 5, 6, 0>(R1, R2);
			vec V2 = Permute<1, 4, 1, 4>(R1, R2);
			matrix M;
			M.v[0] = Permute<0, 4, 5, 3>(V0, V1);
			M.v[1] = Permute<6, 1, 7, 3>(V0, V1);
			M.v[2] = Permute<4, 5, 2, 3>(V0, V2);
			M.v[3] = g_IdentityR3.v;
			return M;
#else
#	if defined(SIMD_SSE2)
			__m128 axis = LoadVec<4>(reinterpret_cast<const float*>(&axis_));

			assert(!(((_mm_movemask_ps(_mm_cmpeq_ps(axis, _mm_setzero_ps())) & 7) == 7) != 0));
			assert(!((_mm_movemask_ps(_mm_cmpeq_ps(_mm_and_ps(axis, g_AbsMask.v), g_Infinity.v)) & 7) != 0));

			__m128 vLengthSq = _mm_mul_ps(axis, axis);
			__m128 vTemp = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(2, 1, 2, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vTemp = _mm_shuffle_ps(vTemp, vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 vResult = _mm_sqrt_ps(vLengthSq);
			__m128 vZeroMask = _mm_setzero_ps();
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			vLengthSq = _mm_cmpneq_ps(vLengthSq, g_Infinity.v);
			vResult = _mm_div_ps(axis, vResult);
			vResult = _mm_and_ps(vResult, vZeroMask);
			__m128 vTemp1 = _mm_andnot_ps(vLengthSq, g_QNaN.v);
			__m128 vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			__m128 Normal = vResult;

			float    fSinAngle;
			float    fCosAngle;
			sin_cos(angle, &fSinAngle, &fCosAngle);

			__m128 C2 = _mm_set_ps1(1.0f - fCosAngle);
			__m128 C1 = _mm_set_ps1(fCosAngle);
			__m128 C0 = _mm_set_ps1(fSinAngle);

			__m128 N0 = _mm_shuffle_ps(Normal, Normal, _MM_SHUFFLE(3, 0, 2, 1));
			__m128 N1 = _mm_shuffle_ps(Normal, Normal, _MM_SHUFFLE(3, 1, 0, 2));

			__m128 V0 = _mm_mul_ps(C2, N0);
			V0 = _mm_mul_ps(V0, N1);

			__m128 R0 = _mm_mul_ps(C2, Normal);
			R0 = _mm_mul_ps(R0, Normal);
			R0 = _mm_add_ps(R0, C1);

			__m128 R1 = _mm_mul_ps(C0, Normal);
			R1 = _mm_add_ps(R1, V0);
			__m128 R2 = _mm_mul_ps(C0, Normal);
			R2 = _mm_sub_ps(V0, R2);

			V0 = _mm_and_ps(R0, g_Mask3.v);
			__m128 V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2, 1, 2, 0));
			V1 = _mm_shuffle_ps(V1, V1, _MM_SHUFFLE(0, 3, 2, 1));
			__m128 V2 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(0, 0, 1, 1));
			V2 = _mm_shuffle_ps(V2, V2, _MM_SHUFFLE(2, 0, 2, 0));

			R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(1, 0, 3, 0));
			R2 = _mm_shuffle_ps(R2, R2, _MM_SHUFFLE(1, 3, 2, 0));

			matrix M;
			M.v[0] = R2;

			R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(3, 2, 3, 1));
			R2 = _mm_shuffle_ps(R2, R2, _MM_SHUFFLE(1, 3, 0, 2));
			M.v[1] = R2;

			V2 = _mm_shuffle_ps(V2, V0, _MM_SHUFFLE(3, 2, 1, 0));
			M.v[2] = V2;
			M.v[3] = g_IdentityR3.v;
			return M;
#	else
			vec axis = LoadVec<4>(reinterpret_cast<const float*>(&axis_));

			assert(!Equal3(axis, vdupq_n_f32(0)));
			assert(!IsInfinite3(axis));

			vec NormalAxis = NormalizeVec<3>(axis);

			float    fSinAngle;
			float    fCosAngle;
			sin_cos(angle, &fSinAngle, &fCosAngle);

			const float tempa = 1.0f - fCosAngle;
			const float tempb = 0.0f;
			float32x2_t _V0 = vcreate_f32(((uint64_t)*(const uint32_t *)&fSinAngle) | ((uint64_t)(*(const uint32_t *)&fCosAngle) << 32));
			float32x2_t _V1 = vcreate_f32(((uint64_t)*(const uint32_t *)&tempa) | ((uint64_t)(*(const uint32_t *)&tempb) << 32));
			vec A = vcombine_f32(_V0, _V1);

			vec C2 = vdupq_lane_f32(vget_high_f32(A.f), 0);
			vec C1 = vdupq_lane_f32(vget_low_f32(A.f), 1);
			vec C0 = vdupq_lane_f32(vget_low_f32(A.f), 0);

			vec N0 = Swizzle<1, 2, 0, 3>(NormalAxis);
			vec N1 = Swizzle<2, 0, 1, 3>(NormalAxis);

			vec V0 = MultiplyVec(C2, N0);
			V0 = MultiplyVec(V0, N1);

			vec R0 = MultiplyVec(C2, NormalAxis);
			R0 = AddVec(MultiplyVec(R0, NormalAxis), C1);

			vec R1 = AddVec(MultiplyVec(C0, NormalAxis), V0);
			vec R2 = SubVec(V0, MultiplyVec(C0, NormalAxis));

			V0 = vbslq_f32(g_Select1110.v.u, R0.f, A.f);
			vec V1 = Permute<2, 5, 6, 0>(R1, R2);
			vec V2 = Permute<1, 4, 1, 4>(R1, R2);
			matrix M;
			M.v[0] = Permute<0, 4, 5, 3>(V0, V1);
			M.v[1] = Permute<6, 1, 7, 3>(V0, V1);
			M.v[2] = Permute<4, 5, 2, 3>(V0, V2);
			M.v[3] = g_IdentityR3.v;
			return M;
#	endif
#endif
		}

		MATH_CALL(matrix) matrix::RotationAxis(const float3& axis_, float angle)
		{
#ifndef MATH_USE_SIMD
			assert(!(axis_.x == 0.f && axis_.y == 0.f && axis_.z == 0.f));
			assert(!(ISINF(axis_.x) || ISINF(axis_.y) || ISINF(axis_.z)));

			vec NormalAxis = NormalizeVec<3>(LoadVec<3>(reinterpret_cast<const float*>(&axis_)));

			float    fSinAngle;
			float    fCosAngle;
			sin_cos(angle, &fSinAngle, &fCosAngle);

			vec A = { fSinAngle, fCosAngle, 1.0f - fCosAngle, 0.0f };

			vec C2 = { A.f[2], A.f[2], A.f[2], A.f[2] };
			vec C1 = { A.f[1], A.f[1], A.f[1], A.f[1] };
			vec C0 = { A.f[0], A.f[0], A.f[0], A.f[0] };

			vec N0 = { NormalAxis.f[1], NormalAxis.f[2], NormalAxis.f[0], NormalAxis.f[3] };
			vec N1 = { NormalAxis.f[2], NormalAxis.f[0], NormalAxis.f[1], NormalAxis.f[3] };

			vec V0 = MultiplyVec(C2, N0);
			V0 = MultiplyVec(V0, N1);

			vec R0 = MultiplyVec(C2, NormalAxis);
			R0 = AddVec(MultiplyVec(R0, NormalAxis), C1);

			vec R1 = AddVec(MultiplyVec(C0, NormalAxis), V0);

			vec R2;
			R2.f[0] = V0.f[0] - (C0.f[0] * NormalAxis.f[0]);
			R2.f[1] = V0.f[1] - (C0.f[1] * NormalAxis.f[1]);
			R2.f[2] = V0.f[2] - (C0.f[2] * NormalAxis.f[2]);
			R2.f[3] = V0.f[3] - (C0.f[3] * NormalAxis.f[3]);

			V0.u[0] = (A.u[0] & ~g_Select1110.v.i[0]) | (R0.u[0] & g_Select1110.v.i[0]);
			V0.u[1] = (A.u[1] & ~g_Select1110.v.i[1]) | (R0.u[1] & g_Select1110.v.i[1]);
			V0.u[2] = (A.u[2] & ~g_Select1110.v.i[2]) | (R0.u[2] & g_Select1110.v.i[2]);
			V0.u[3] = (A.u[3] & ~g_Select1110.v.i[3]) | (R0.u[3] & g_Select1110.v.i[3]);

			vec V1 = Permute<2, 5, 6, 0>(R1, R2);
			vec V2 = Permute<1, 4, 1, 4>(R1, R2);
			matrix M;
			M.v[0] = Permute<0, 4, 5, 3>(V0, V1);
			M.v[1] = Permute<6, 1, 7, 3>(V0, V1);
			M.v[2] = Permute<4, 5, 2, 3>(V0, V2);
			M.v[3] = g_IdentityR3.v;
			return M;
#else
#	if defined(SIMD_SSE2)
			__m128 axis = LoadVec<3>(reinterpret_cast<const float*>(&axis_));

			assert(!(((_mm_movemask_ps(_mm_cmpeq_ps(axis, _mm_setzero_ps())) & 7) == 7) != 0));
			assert(!((_mm_movemask_ps(_mm_cmpeq_ps(_mm_and_ps(axis, g_AbsMask.v), g_Infinity.v)) & 7) != 0));

			__m128 vLengthSq = _mm_mul_ps(axis, axis);
			__m128 vTemp = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(2, 1, 2, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vTemp = _mm_shuffle_ps(vTemp, vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 vResult = _mm_sqrt_ps(vLengthSq);
			__m128 vZeroMask = _mm_setzero_ps();
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			vLengthSq = _mm_cmpneq_ps(vLengthSq, g_Infinity.v);
			vResult = _mm_div_ps(axis, vResult);
			vResult = _mm_and_ps(vResult, vZeroMask);
			__m128 vTemp1 = _mm_andnot_ps(vLengthSq, g_QNaN.v);
			__m128 vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			__m128 Normal = vResult;

			float    fSinAngle;
			float    fCosAngle;
			sin_cos(angle, &fSinAngle, &fCosAngle);

			__m128 C2 = _mm_set_ps1(1.0f - fCosAngle);
			__m128 C1 = _mm_set_ps1(fCosAngle);
			__m128 C0 = _mm_set_ps1(fSinAngle);

			__m128 N0 = _mm_shuffle_ps(Normal, Normal, _MM_SHUFFLE(3, 0, 2, 1));
			__m128 N1 = _mm_shuffle_ps(Normal, Normal, _MM_SHUFFLE(3, 1, 0, 2));

			__m128 V0 = _mm_mul_ps(C2, N0);
			V0 = _mm_mul_ps(V0, N1);

			__m128 R0 = _mm_mul_ps(C2, Normal);
			R0 = _mm_mul_ps(R0, Normal);
			R0 = _mm_add_ps(R0, C1);

			__m128 R1 = _mm_mul_ps(C0, Normal);
			R1 = _mm_add_ps(R1, V0);
			__m128 R2 = _mm_mul_ps(C0, Normal);
			R2 = _mm_sub_ps(V0, R2);

			V0 = _mm_and_ps(R0, g_Mask3.v);
			__m128 V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2, 1, 2, 0));
			V1 = _mm_shuffle_ps(V1, V1, _MM_SHUFFLE(0, 3, 2, 1));
			__m128 V2 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(0, 0, 1, 1));
			V2 = _mm_shuffle_ps(V2, V2, _MM_SHUFFLE(2, 0, 2, 0));

			R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(1, 0, 3, 0));
			R2 = _mm_shuffle_ps(R2, R2, _MM_SHUFFLE(1, 3, 2, 0));

			matrix M;
			M.v[0] = R2;

			R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(3, 2, 3, 1));
			R2 = _mm_shuffle_ps(R2, R2, _MM_SHUFFLE(1, 3, 0, 2));
			M.v[1] = R2;

			V2 = _mm_shuffle_ps(V2, V0, _MM_SHUFFLE(3, 2, 1, 0));
			M.v[2] = V2;
			M.v[3] = g_IdentityR3.v;
			return M;
#	else
			vec axis = LoadVec<3>(reinterpret_cast<const float*>(&axis_));

			assert(!Equal3(axis, vdupq_n_f32(0)));
			assert(!IsInfinite3(axis));

			vec NormalAxis = NormalizeVec<3>(axis);

			float    fSinAngle;
			float    fCosAngle;
			sin_cos(angle, &fSinAngle, &fCosAngle);

			const float tempa = 1.0f - fCosAngle;
			const float tempb = 0.0f;
			float32x2_t _V0 = vcreate_f32(((uint64_t)*(const uint32_t *)&fSinAngle) | ((uint64_t)(*(const uint32_t *)&fCosAngle) << 32));
			float32x2_t _V1 = vcreate_f32(((uint64_t)*(const uint32_t *)&tempa) | ((uint64_t)(*(const uint32_t *)&tempb) << 32));
			vec A = vcombine_f32(_V0, _V1);

			vec C2 = vdupq_lane_f32(vget_high_f32(A.f), 0);
			vec C1 = vdupq_lane_f32(vget_low_f32(A.f), 1);
			vec C0 = vdupq_lane_f32(vget_low_f32(A.f), 0);

			vec N0 = Swizzle<1, 2, 0, 3>(NormalAxis);
			vec N1 = Swizzle<2, 0, 1, 3>(NormalAxis);

			vec V0 = MultiplyVec(C2, N0);
			V0 = MultiplyVec(V0, N1);

			vec R0 = MultiplyVec(C2, NormalAxis);
			R0 = AddVec(MultiplyVec(R0, NormalAxis), C1);

			vec R1 = AddVec(MultiplyVec(C0, NormalAxis), V0);
			vec R2 = SubVec(V0, MultiplyVec(C0, NormalAxis));

			V0 = vbslq_f32(g_Select1110.v.u, R0.f, A.f);
			vec V1 = Permute<2, 5, 6, 0>(R1, R2);
			vec V2 = Permute<1, 4, 1, 4>(R1, R2);
			matrix M;
			M.v[0] = Permute<0, 4, 5, 3>(V0, V1);
			M.v[1] = Permute<6, 1, 7, 3>(V0, V1);
			M.v[2] = Permute<4, 5, 2, 3>(V0, V2);
			M.v[3] = g_IdentityR3.v;
			return M;
#	endif
#endif
		}

		MATH_CALL(matrix) matrix::RotationAxis(vec_f axis, float angle)
		{
#ifndef MATH_USE_SIMD
			assert(!(axis.f[0] == 0.f && axis.f[1] == 0.f && axis.f[2] == 0.f));
			assert(!(ISINF(axis.f[0]) || ISINF(axis.f[1]) || ISINF(axis.f[2])));

			vec NormalAxis = NormalizeVec<3>(axis);

			float    fSinAngle;
			float    fCosAngle;
			sin_cos(angle, &fSinAngle, &fCosAngle);

			vec A = { fSinAngle, fCosAngle, 1.0f - fCosAngle, 0.0f };

			vec C2 = { A.f[2], A.f[2], A.f[2], A.f[2] };
			vec C1 = { A.f[1], A.f[1], A.f[1], A.f[1] };
			vec C0 = { A.f[0], A.f[0], A.f[0], A.f[0] };

			vec N0 = { NormalAxis.f[1], NormalAxis.f[2], NormalAxis.f[0], NormalAxis.f[3] };
			vec N1 = { NormalAxis.f[2], NormalAxis.f[0], NormalAxis.f[1], NormalAxis.f[3] };

			vec V0 = MultiplyVec(C2, N0);
			V0 = MultiplyVec(V0, N1);

			vec R0 = MultiplyVec(C2, NormalAxis);
			R0 = AddVec(MultiplyVec(R0, NormalAxis), C1);

			vec R1 = AddVec(MultiplyVec(C0, NormalAxis), V0);

			vec R2;
			R2.f[0] = V0.f[0] - (C0.f[0] * NormalAxis.f[0]);
			R2.f[1] = V0.f[1] - (C0.f[1] * NormalAxis.f[1]);
			R2.f[2] = V0.f[2] - (C0.f[2] * NormalAxis.f[2]);
			R2.f[3] = V0.f[3] - (C0.f[3] * NormalAxis.f[3]);

			V0.u[0] = (A.u[0] & ~g_Select1110.v.i[0]) | (R0.u[0] & g_Select1110.v.i[0]);
			V0.u[1] = (A.u[1] & ~g_Select1110.v.i[1]) | (R0.u[1] & g_Select1110.v.i[1]);
			V0.u[2] = (A.u[2] & ~g_Select1110.v.i[2]) | (R0.u[2] & g_Select1110.v.i[2]);
			V0.u[3] = (A.u[3] & ~g_Select1110.v.i[3]) | (R0.u[3] & g_Select1110.v.i[3]);

			vec V1 = Permute<2, 5, 6, 0>(R1, R2);
			vec V2 = Permute<1, 4, 1, 4>(R1, R2);
			matrix M;
			M.v[0] = Permute<0, 4, 5, 3>(V0, V1);
			M.v[1] = Permute<6, 1, 7, 3>(V0, V1);
			M.v[2] = Permute<4, 5, 2, 3>(V0, V2);
			M.v[3] = g_IdentityR3.v;
			return M;
#else
#	if defined(SIMD_SSE2)
			assert(!(((_mm_movemask_ps(_mm_cmpeq_ps(axis, _mm_setzero_ps())) & 7) == 7) != 0));
			assert(!((_mm_movemask_ps(_mm_cmpeq_ps(_mm_and_ps(axis, g_AbsMask.v), g_Infinity.v)) & 7) != 0));

			__m128 vLengthSq = _mm_mul_ps(axis, axis);
			__m128 vTemp = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(2, 1, 2, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vTemp = _mm_shuffle_ps(vTemp, vTemp, _MM_SHUFFLE(1, 1, 1, 1));
			vLengthSq = _mm_add_ss(vLengthSq, vTemp);
			vLengthSq = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 vResult = _mm_sqrt_ps(vLengthSq);
			__m128 vZeroMask = _mm_setzero_ps();
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			vLengthSq = _mm_cmpneq_ps(vLengthSq, g_Infinity.v);
			vResult = _mm_div_ps(axis, vResult);
			vResult = _mm_and_ps(vResult, vZeroMask);
			__m128 vTemp1 = _mm_andnot_ps(vLengthSq, g_QNaN.v);
			__m128 vTemp2 = _mm_and_ps(vResult, vLengthSq);
			vResult = _mm_or_ps(vTemp1, vTemp2);
			__m128 Normal = vResult;

			float    fSinAngle;
			float    fCosAngle;
			sin_cos(angle, &fSinAngle, &fCosAngle);

			__m128 C2 = _mm_set_ps1(1.0f - fCosAngle);
			__m128 C1 = _mm_set_ps1(fCosAngle);
			__m128 C0 = _mm_set_ps1(fSinAngle);

			__m128 N0 = _mm_shuffle_ps(Normal, Normal, _MM_SHUFFLE(3, 0, 2, 1));
			__m128 N1 = _mm_shuffle_ps(Normal, Normal, _MM_SHUFFLE(3, 1, 0, 2));

			__m128 V0 = _mm_mul_ps(C2, N0);
			V0 = _mm_mul_ps(V0, N1);

			__m128 R0 = _mm_mul_ps(C2, Normal);
			R0 = _mm_mul_ps(R0, Normal);
			R0 = _mm_add_ps(R0, C1);

			__m128 R1 = _mm_mul_ps(C0, Normal);
			R1 = _mm_add_ps(R1, V0);
			__m128 R2 = _mm_mul_ps(C0, Normal);
			R2 = _mm_sub_ps(V0, R2);

			V0 = _mm_and_ps(R0, g_Mask3.v);
			__m128 V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2, 1, 2, 0));
			V1 = _mm_shuffle_ps(V1, V1, _MM_SHUFFLE(0, 3, 2, 1));
			__m128 V2 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(0, 0, 1, 1));
			V2 = _mm_shuffle_ps(V2, V2, _MM_SHUFFLE(2, 0, 2, 0));

			R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(1, 0, 3, 0));
			R2 = _mm_shuffle_ps(R2, R2, _MM_SHUFFLE(1, 3, 2, 0));

			matrix M;
			M.v[0] = R2;

			R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(3, 2, 3, 1));
			R2 = _mm_shuffle_ps(R2, R2, _MM_SHUFFLE(1, 3, 0, 2));
			M.v[1] = R2;

			V2 = _mm_shuffle_ps(V2, V0, _MM_SHUFFLE(3, 2, 1, 0));
			M.v[2] = V2;
			M.v[3] = g_IdentityR3.v;
			return M;
#	else
			assert(!Equal3(axis, vdupq_n_f32(0)));
			assert(!IsInfinite3(axis));

			vec NormalAxis = NormalizeVec<3>(axis);

			float    fSinAngle;
			float    fCosAngle;
			sin_cos(angle, &fSinAngle, &fCosAngle);

			const float tempa = 1.0f - fCosAngle;
			const float tempb = 0.0f;
			float32x2_t _V0 = vcreate_f32(((uint64_t)*(const uint32_t *)&fSinAngle) | ((uint64_t)(*(const uint32_t *)&fCosAngle) << 32));
			float32x2_t _V1 = vcreate_f32(((uint64_t)*(const uint32_t *)&tempa) | ((uint64_t)(*(const uint32_t *)&tempb) << 32));
			vec A = vcombine_f32(_V0, _V1);

			vec C2 = vdupq_lane_f32(vget_high_f32(A.f), 0);
			vec C1 = vdupq_lane_f32(vget_low_f32(A.f), 1);
			vec C0 = vdupq_lane_f32(vget_low_f32(A.f), 0);

			vec N0 = Swizzle<1, 2, 0, 3>(NormalAxis);
			vec N1 = Swizzle<2, 0, 1, 3>(NormalAxis);

			vec V0 = MultiplyVec(C2, N0);
			V0 = MultiplyVec(V0, N1);

			vec R0 = MultiplyVec(C2, NormalAxis);
			R0 = AddVec(MultiplyVec(R0, NormalAxis), C1);

			vec R1 = AddVec(MultiplyVec(C0, NormalAxis), V0);
			vec R2 = SubVec(V0, MultiplyVec(C0, NormalAxis));

			V0 = vbslq_f32(g_Select1110.v.u, R0.f, A.f);
			vec V1 = Permute<2, 5, 6, 0>(R1, R2);
			vec V2 = Permute<1, 4, 1, 4>(R1, R2);
			matrix M;
			M.v[0] = Permute<0, 4, 5, 3>(V0, V1);
			M.v[1] = Permute<6, 1, 7, 3>(V0, V1);
			M.v[2] = Permute<4, 5, 2, 3>(V0, V2);
			M.v[3] = g_IdentityR3.v;
			return M;
#	endif
#endif
		}

		inline matrix LookToLHTemp(vec_f EyePos, vec_f Eye, vec_f Up)
		{
#ifndef MATH_USE_SIMD
			assert(!(Eye.f[0] == 0.f && Eye.f[1] == 0.f && Eye.f[2] == 0.f));
			assert(!(ISINF(Eye.f[0]) || ISINF(Eye.f[1]) || ISINF(Eye.f[2])));
			assert(!(Up.f[0] == 0.f && Up.f[1] == 0.f && Up.f[2] == 0.f));
			assert(!(ISINF(Up.f[0]) || ISINF(Up.f[1]) || ISINF(Up.f[2])));

			vec R2 = NormalizeVec<3>(Eye);

			vec R0 = CrossVec<3>(Up, R2);
			R0 = NormalizeVec<3>(R0);

			vec R1 = CrossVec<3>(R2, R0);

			vec NegEyePosition;
			NegEyePosition.f[0] = -EyePos.f[0];
			NegEyePosition.f[1] = -EyePos.f[1];
			NegEyePosition.f[2] = -EyePos.f[2];
			NegEyePosition.f[3] = -EyePos.f[3];

			vec D0 = DotVec<3>(R0, NegEyePosition);
			vec D1 = DotVec<3>(R1, NegEyePosition);
			vec D2 = DotVec<3>(R2, NegEyePosition);

			matrix M;
			M[0].u[0] = (D0.u[0] & ~g_Select1110.v.u[0]) | (R0.u[0] & g_Select1110.v.u[0]);
			M[0].u[1] = (D0.u[1] & ~g_Select1110.v.u[1]) | (R0.u[1] & g_Select1110.v.u[1]);
			M[0].u[2] = (D0.u[2] & ~g_Select1110.v.u[2]) | (R0.u[2] & g_Select1110.v.u[2]);
			M[0].u[3] = (D0.u[3] & ~g_Select1110.v.u[3]) | (R0.u[3] & g_Select1110.v.u[3]);

			M[1].u[0] = (D1.u[0] & ~g_Select1110.v.u[0]) | (R1.u[0] & g_Select1110.v.u[0]);
			M[1].u[1] = (D1.u[1] & ~g_Select1110.v.u[1]) | (R1.u[1] & g_Select1110.v.u[1]);
			M[1].u[2] = (D1.u[2] & ~g_Select1110.v.u[2]) | (R1.u[2] & g_Select1110.v.u[2]);
			M[1].u[3] = (D1.u[3] & ~g_Select1110.v.u[3]) | (R1.u[3] & g_Select1110.v.u[3]);

			M[2].u[0] = (D2.u[0] & ~g_Select1110.v.u[0]) | (R2.u[0] & g_Select1110.v.u[0]);
			M[2].u[1] = (D2.u[1] & ~g_Select1110.v.u[1]) | (R2.u[1] & g_Select1110.v.u[1]);
			M[2].u[2] = (D2.u[2] & ~g_Select1110.v.u[2]) | (R2.u[2] & g_Select1110.v.u[2]);
			M[2].u[3] = (D2.u[3] & ~g_Select1110.v.u[3]) | (R2.u[3] & g_Select1110.v.u[3]);

			M[3] = g_IdentityR3.v;

			return M.TransposeMatrix();
#else
#	if defined(SIMD_SSE2)
			assert(!(((_mm_movemask_ps(_mm_cmpeq_ps(Eye, _mm_setzero_ps())) & 7) == 7) != 0));
			assert(!((_mm_movemask_ps(_mm_cmpeq_ps(_mm_and_ps(Eye, g_AbsMask.v), g_Infinity.v)) & 7) != 0));
			assert(!(((_mm_movemask_ps(_mm_cmpeq_ps(Up, _mm_setzero_ps())) & 7) == 7) != 0));
			assert(!((_mm_movemask_ps(_mm_cmpeq_ps(_mm_and_ps(Up, g_AbsMask.v), g_Infinity.v)) & 7) != 0));

			__m128 R2 = NormalizeVec<3>(Eye);

			__m128 R0 = CrossVec<3>(Up, R2);
			R0 = NormalizeVec<3>(R0);

			__m128 R1 = CrossVec<3>(R2, R0);
			__m128 NegEyePosition = _mm_sub_ps(_mm_setzero_ps(), EyePos);

			__m128 D0 = DotVec<3>(R0, NegEyePosition);
			__m128 D1 = DotVec<3>(R1, NegEyePosition);
			__m128 D2 = DotVec<3>(R2, NegEyePosition);

			matrix M;

			M[0] = _mm_or_ps(_mm_andnot_ps(g_Select1110.v, D0), _mm_and_ps(R0, g_Select1110.v));
			M[1] = _mm_or_ps(_mm_andnot_ps(g_Select1110.v, D1), _mm_and_ps(R1, g_Select1110.v));
			M[2] = _mm_or_ps(_mm_andnot_ps(g_Select1110.v, D2), _mm_and_ps(R2, g_Select1110.v));
			M[3] = g_IdentityR3.v;

			return M.TransposeMatrix();
#	else
			assert(!Equal3(Eye, vdupq_n_f32(0)));
			assert(!IsInfinite3(Eye));
			assert(!Equal3(Up, vdupq_n_f32(0)));
			assert(!IsInfinite3(Up));

			vec R2 = NormalizeVec<3>(Eye);

			vec R0 = CrossVec<3>(Up, R2);
			R0 = NormalizeVec<3>(R0);

			vec R1 = CrossVec<3>(R2, R0);

			vec NegEyePosition = vnegq_f32(EyePos.f);

			vec D0 = DotVec<3>(R0, NegEyePosition);
			vec D1 = DotVec<3>(R1, NegEyePosition);
			vec D2 = DotVec<3>(R2, NegEyePosition);

			matrix M;
			M[0] = vbslq_f32(g_Select1110.v.u, R0.f, D0.f);
			M[1] = vbslq_f32(g_Select1110.v.u, R1.f, D1.f);
			M[2] = vbslq_f32(g_Select1110.v.u, R2.f, D2.f);
			M[3] = g_IdentityR3.v;

			return M.TransposeMatrix();
#	endif
#endif
		}

		MATH_CALL(matrix) matrix::LookAtLH(const float4& EyePos, const float4& FocusPos, const float4& Up)
		{
#ifndef MATH_USE_SIMD
			vec EyeDirection;
			EyeDirection.f[0] = FocusPos.x - EyePos.x;
			EyeDirection.f[1] = FocusPos.y - EyePos.y;
			EyeDirection.f[2] = FocusPos.z - EyePos.z;
			EyeDirection.f[3] = FocusPos.w - EyePos.w;
#else
#	if defined(SIMD_SSE2)
			vec EyeDirection = _mm_sub_ps(
				LoadVec<4>(reinterpret_cast<const float*>(&FocusPos)),
				LoadVec<4>(reinterpret_cast<const float*>(&EyePos))
			);
#	else
			vec EyeDirection = SubVec(
				LoadVec<4>(reinterpret_cast<const float*>(&FocusPos)),
				LoadVec<4>(reinterpret_cast<const float*>(&EyePos))
			);
#	endif
#endif
			return LookToLHTemp(
				LoadVec<4>(reinterpret_cast<const float*>(&EyePos)),
				EyeDirection,
				LoadVec<4>(reinterpret_cast<const float*>(&Up))
			);
		}

		MATH_CALL(matrix) matrix::LookAtRH(const float4& EyePos, const float4& FocusPos, const float4& Up)
		{
#ifndef MATH_USE_SIMD
			vec NegEyeDirection;
			NegEyeDirection.f[0] = -(FocusPos.x - EyePos.x);
			NegEyeDirection.f[1] = -(FocusPos.y - EyePos.y);
			NegEyeDirection.f[2] = -(FocusPos.z - EyePos.z);
			NegEyeDirection.f[3] = -(FocusPos.w - EyePos.w);
#else
#	if defined(SIMD_SSE2)
			vec NegEyeDirection = _mm_sub_ps(
				LoadVec<4>(reinterpret_cast<const float*>(&EyePos)),
				LoadVec<4>(reinterpret_cast<const float*>(&FocusPos))
			);
#	else
			vec NegEyeDirection = SubVec(
				LoadVec<4>(reinterpret_cast<const float*>(&EyePos)),
				LoadVec<4>(reinterpret_cast<const float*>(&FocusPos))
			);
#	endif
#endif
			return LookToLHTemp(
				LoadVec<4>(reinterpret_cast<const float*>(&EyePos)),
				NegEyeDirection,
				LoadVec<4>(reinterpret_cast<const float*>(&Up))
			);
		}

		MATH_CALL(matrix) matrix::LookToLH(const float4& EyePos, const float4& Eye, const float4& Up)
		{
			return LookToLHTemp(
				LoadVec<4>(reinterpret_cast<const float*>(&EyePos)),
				LoadVec<4>(reinterpret_cast<const float*>(&Eye)),
				LoadVec<4>(reinterpret_cast<const float*>(&Up))
			);
		}

		MATH_CALL(matrix) matrix::LookToRH(const float4& EyePos, const float4& Eye, const float4& Up)
		{
#ifndef MATH_USE_SIMD
			vec NegEyeDirection;
			NegEyeDirection.f[0] = -Eye.x;
			NegEyeDirection.f[1] = -Eye.y;
			NegEyeDirection.f[2] = -Eye.z;
			NegEyeDirection.f[3] = -Eye.w;
#else
#	if defined(SIMD_SSE2)
			vec NegEyeDirection = _mm_sub_ps(_mm_setzero_ps(),
				LoadVec<4>(reinterpret_cast<const float*>(&Eye)));
#	else
			vec NegEyeDirection = vnegq_f32(LoadVec<4>(reinterpret_cast<const float*>(&Eye)).f);
#	endif
#endif
			return LookToLHTemp(
				LoadVec<4>(reinterpret_cast<const float*>(&EyePos)),
				NegEyeDirection,
				LoadVec<4>(reinterpret_cast<const float*>(&Up))
			);
		}
		
		MATH_CALL(matrix) matrix::PerspectiveLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
		{
			assert(NearZ > 0.f && FarZ > 0.f);

			assert(!(Abs(ViewWidth - 0.0f) <= 0.00001f));
			assert(!(Abs(ViewHeight - 0.0f) <= 0.00001f));
			assert(!(Abs(FarZ - NearZ) <= 0.00001f));
#ifndef MATH_USE_SIMD
			float TwoNearZ = NearZ + NearZ;
			float fRange = FarZ / (FarZ - NearZ);

			matrix M;
			M.v[0].f[0] = TwoNearZ / ViewWidth;
			M.v[0].f[1] = 0.0f;
			M.v[0].f[2] = 0.0f;
			M.v[0].f[3] = 0.0f;

			M.v[1].f[0] = 0.0f;
			M.v[1].f[1] = TwoNearZ / ViewHeight;
			M.v[1].f[2] = 0.0f;
			M.v[1].f[3] = 0.0f;

			M.v[2].f[0] = 0.0f;
			M.v[2].f[1] = 0.0f;
			M.v[2].f[2] = fRange;
			M.v[2].f[3] = 1.0f;

			M.v[3].f[0] = 0.0f;
			M.v[3].f[1] = 0.0f;
			M.v[3].f[2] = -fRange * NearZ;
			M.v[3].f[3] = 0.0f;
			return M;
#else
#	if defined(SIMD_SSE2)

			matrix M;
			float TwoNearZ = NearZ + NearZ;
			float fRange = FarZ / (FarZ - NearZ);
			__m128 rMem = {
				TwoNearZ / ViewWidth,
				TwoNearZ / ViewHeight,
				fRange,
				-fRange * NearZ
			};
			__m128 vValues = rMem;
			__m128 vTemp = _mm_setzero_ps();
			vTemp = _mm_move_ss(vTemp, vValues);
			M.v[0] = vTemp;
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_MaskY.v);
			M.v[1] = vTemp;
			vValues = _mm_shuffle_ps(vValues, g_IdentityR3.v, _MM_SHUFFLE(3, 2, 3, 2));
			vTemp = _mm_setzero_ps();
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
			M.v[2] = vTemp;
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
			M.v[3] = vTemp;

			return M;
#	else
			float TwoNearZ = NearZ + NearZ;
			float fRange = FarZ / (FarZ - NearZ);
			const vec Zero = vdupq_n_f32(0);
			matrix M;
			M.v[0] = vsetq_lane_f32(TwoNearZ / ViewWidth, Zero.f, 0);
			M.v[1] = vsetq_lane_f32(TwoNearZ / ViewHeight, Zero.f, 1);
			M.v[2] = vsetq_lane_f32(fRange, g_IdentityR3.v.f, 2);
			M.v[3] = vsetq_lane_f32(-fRange * NearZ, Zero.f, 2);
			return M;
#	endif
#endif
		}
		
		MATH_CALL(matrix) matrix::PerspectiveRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
		{
			assert(NearZ > 0.f && FarZ > 0.f);

			assert(!(Abs(ViewWidth - 0.0f) <= 0.00001f));
			assert(!(Abs(ViewHeight - 0.0f) <= 0.00001f));
			assert(!(Abs(FarZ - NearZ) <= 0.00001f));

#ifndef MATH_USE_SIMD

			float TwoNearZ = NearZ + NearZ;
			float fRange = FarZ / (NearZ - FarZ);

			matrix M;
			M.v[0].f[0] = TwoNearZ / ViewWidth;
			M.v[0].f[1] = 0.0f;
			M.v[0].f[2] = 0.0f;
			M.v[0].f[3] = 0.0f;

			M.v[1].f[0] = 0.0f;
			M.v[1].f[1] = TwoNearZ / ViewHeight;
			M.v[1].f[2] = 0.0f;
			M.v[1].f[3] = 0.0f;

			M.v[2].f[0] = 0.0f;
			M.v[2].f[1] = 0.0f;
			M.v[2].f[2] = fRange;
			M.v[2].f[3] = -1.0f;

			M.v[3].f[0] = 0.0f;
			M.v[3].f[1] = 0.0f;
			M.v[3].f[2] = fRange * NearZ;
			M.v[3].f[3] = 0.0f;
			return M;
#else
#	if defined(SIMD_SSE2)
			matrix M;
			float TwoNearZ = NearZ + NearZ;
			float fRange = FarZ / (NearZ - FarZ);
			__m128 rMem = {
				TwoNearZ / ViewWidth,
				TwoNearZ / ViewHeight,
				fRange,
				fRange * NearZ
			};
			__m128 vValues = rMem;
			__m128 vTemp = _mm_setzero_ps();
			vTemp = _mm_move_ss(vTemp, vValues);
			M.v[0] = vTemp;
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_MaskY.v);
			M.v[1] = vTemp;
			vValues = _mm_shuffle_ps(vValues, g_NegIdentityR3.v, _MM_SHUFFLE(3, 2, 3, 2));
			vTemp = _mm_setzero_ps();
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
			M.v[2] = vTemp;
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
			M.v[3] = vTemp;
			return M;
#	else
			float TwoNearZ = NearZ + NearZ;
			float fRange = FarZ / (NearZ - FarZ);
			const vec Zero = vdupq_n_f32(0);

			matrix M;
			M.v[0] = vsetq_lane_f32(TwoNearZ / ViewWidth, Zero.f, 0);
			M.v[1] = vsetq_lane_f32(TwoNearZ / ViewHeight, Zero.f, 1);
			M.v[2] = vsetq_lane_f32(fRange, g_NegIdentityR3.v.f, 2);
			M.v[3] = vsetq_lane_f32(fRange * NearZ, Zero.f, 2);
			return M;
#	endif
#endif

		}
		
		MATH_CALL(matrix) matrix::PerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)
		{
			assert(NearZ > 0.f && FarZ > 0.f);
			assert(!(Abs(FovAngleY - 0.0f) <= 0.00001f * 2.0f));
			assert(!(Abs(AspectRatio - 0.0f) <= 0.00001f));
			assert(!(Abs(FarZ - NearZ) <= 0.00001f));

#ifndef MATH_USE_SIMD
			float    SinFov;
			float    CosFov;
			sin_cos(0.5f * FovAngleY, &SinFov, &CosFov);

			float Height = CosFov / SinFov;
			float Width = Height / AspectRatio;
			float fRange = FarZ / (FarZ - NearZ);

			matrix M;
			M.v[0].f[0] = Width;
			M.v[0].f[1] = 0.0f;
			M.v[0].f[2] = 0.0f;
			M.v[0].f[3] = 0.0f;

			M.v[1].f[0] = 0.0f;
			M.v[1].f[1] = Height;
			M.v[1].f[2] = 0.0f;
			M.v[1].f[3] = 0.0f;

			M.v[2].f[0] = 0.0f;
			M.v[2].f[1] = 0.0f;
			M.v[2].f[2] = fRange;
			M.v[2].f[3] = 1.0f;

			M.v[3].f[0] = 0.0f;
			M.v[3].f[1] = 0.0f;
			M.v[3].f[2] = -fRange * NearZ;
			M.v[3].f[3] = 0.0f;
			return M;

#else
#	if defined(SIMD_SSE2)
			float    SinFov;
			float    CosFov;
			sin_cos(0.5f * FovAngleY, &SinFov, &CosFov);

			float fRange = FarZ / (FarZ - NearZ);
			float Height = CosFov / SinFov;
			__m128 rMem = {
				Height / AspectRatio,
				Height,
				fRange,
				-fRange * NearZ
			};
			__m128 vValues = rMem;
			__m128 vTemp = _mm_setzero_ps();
			vTemp = _mm_move_ss(vTemp, vValues);
			matrix M;
			M.v[0] = vTemp;
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_MaskY.v);
			M.v[1] = vTemp;
			vTemp = _mm_setzero_ps();
			vValues = _mm_shuffle_ps(vValues, g_IdentityR3.v, _MM_SHUFFLE(3, 2, 3, 2));
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
			M.v[2] = vTemp;
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
			M.v[3] = vTemp;
			return M;
#	else
			float    SinFov;
			float    CosFov;
			sin_cos(0.5f * FovAngleY, &SinFov, &CosFov);

			float fRange = FarZ / (FarZ - NearZ);
			float Height = CosFov / SinFov;
			float Width = Height / AspectRatio;
			const vec Zero = vdupq_n_f32(0);

			matrix M;
			M.v[0] = vsetq_lane_f32(Width, Zero.f, 0);
			M.v[1] = vsetq_lane_f32(Height, Zero.f, 1);
			M.v[2] = vsetq_lane_f32(fRange, g_IdentityR3.v.f, 2);
			M.v[3] = vsetq_lane_f32(-fRange * NearZ, Zero.f, 2);
			return M;
#	endif
#endif

		}
		
		MATH_CALL(matrix) matrix::PerspectiveFovRH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)
		{
			assert(NearZ > 0.f && FarZ > 0.f);
			assert(!(Abs(FovAngleY - 0.0f) <= 0.00001f * 2.0f));
			assert(!(Abs(AspectRatio - 0.0f) <= 0.00001f));
			assert(!(Abs(FarZ - NearZ) <= 0.00001f));

#ifndef MATH_USE_SIMD
			float    SinFov;
			float    CosFov;
			sin_cos(0.5f * FovAngleY, &SinFov, &CosFov);

			float Height = CosFov / SinFov;
			float Width = Height / AspectRatio;
			float fRange = FarZ / (NearZ - FarZ);

			matrix M;
			M.v[0].f[0] = Width;
			M.v[0].f[1] = 0.0f;
			M.v[0].f[2] = 0.0f;
			M.v[0].f[3] = 0.0f;

			M.v[1].f[0] = 0.0f;
			M.v[1].f[1] = Height;
			M.v[1].f[2] = 0.0f;
			M.v[1].f[3] = 0.0f;

			M.v[2].f[0] = 0.0f;
			M.v[2].f[1] = 0.0f;
			M.v[2].f[2] = fRange;
			M.v[2].f[3] = -1.0f;

			M.v[3].f[0] = 0.0f;
			M.v[3].f[1] = 0.0f;
			M.v[3].f[2] = fRange * NearZ;
			M.v[3].f[3] = 0.0f;
			return M;
#else
#	if defined(SIMD_SSE2)
			float    SinFov;
			float    CosFov;
			sin_cos(0.5f * FovAngleY, &SinFov, &CosFov);
			float fRange = FarZ / (NearZ - FarZ);
			float Height = CosFov / SinFov;
			__m128 rMem = {
				Height / AspectRatio,
				Height,
				fRange,
				fRange * NearZ
			};
			__m128 vValues = rMem;
			__m128 vTemp = _mm_setzero_ps();
			vTemp = _mm_move_ss(vTemp, vValues);
			matrix M;
			M.v[0] = vTemp;
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_MaskY.v);
			M.v[1] = vTemp;
			vTemp = _mm_setzero_ps();
			vValues = _mm_shuffle_ps(vValues, g_NegIdentityR3.v, _MM_SHUFFLE(3, 2, 3, 2));
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
			M.v[2] = vTemp;
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
			M.v[3] = vTemp;
			return M;
#	else
			float    SinFov;
			float    CosFov;
			sin_cos(0.5f * FovAngleY, &SinFov, &CosFov);
			float fRange = FarZ / (NearZ - FarZ);
			float Height = CosFov / SinFov;
			float Width = Height / AspectRatio;
			const vec Zero = vdupq_n_f32(0);

			matrix M;
			M.v[0] = vsetq_lane_f32(Width, Zero.f, 0);
			M.v[1] = vsetq_lane_f32(Height, Zero.f, 1);
			M.v[2] = vsetq_lane_f32(fRange, g_NegIdentityR3.v.f, 2);
			M.v[3] = vsetq_lane_f32(fRange * NearZ, Zero.f, 2);
			return M;
#	endif
#endif

		}
		
		MATH_CALL(matrix) matrix::PerspectiveOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)
		{
			assert(NearZ > 0.f && FarZ > 0.f);

			assert(!(Abs(ViewRight - ViewLeft) <= 0.00001f));
			assert(!(Abs(ViewTop - ViewBottom) <= 0.00001f));
			assert(!(Abs(FarZ - NearZ) <= 0.00001f));
#ifndef MATH_USE_SIMD
			float TwoNearZ = NearZ + NearZ;
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = FarZ / (FarZ - NearZ);

			matrix M;
			M.v[0].f[0] = TwoNearZ * ReciprocalWidth;
			M.v[0].f[1] = 0.0f;
			M.v[0].f[2] = 0.0f;
			M.v[0].f[3] = 0.0f;

			M.v[1].f[0] = 0.0f;
			M.v[1].f[1] = TwoNearZ * ReciprocalHeight;
			M.v[1].f[2] = 0.0f;
			M.v[1].f[3] = 0.0f;

			M.v[2].f[0] = -(ViewLeft + ViewRight) * ReciprocalWidth;
			M.v[2].f[1] = -(ViewTop + ViewBottom) * ReciprocalHeight;
			M.v[2].f[2] = fRange;
			M.v[2].f[3] = 1.0f;

			M.v[3].f[0] = 0.0f;
			M.v[3].f[1] = 0.0f;
			M.v[3].f[2] = -fRange * NearZ;
			M.v[3].f[3] = 0.0f;
			return M;
#else
#	if defined(SIMD_SSE2)
			matrix M;
			float TwoNearZ = NearZ + NearZ;
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = FarZ / (FarZ - NearZ);
			__m128 rMem = {
				TwoNearZ*ReciprocalWidth,
				TwoNearZ*ReciprocalHeight,
				-fRange * NearZ,
				0
			};
			__m128 vValues = rMem;
			__m128 vTemp = _mm_setzero_ps();
			vTemp = _mm_move_ss(vTemp, vValues);
			M.v[0] = vTemp;
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_MaskY.v);
			M.v[1] = vTemp;
			M.v[2] = _mm_set_ps(1.0f,
				fRange,
				-(ViewTop + ViewBottom) * ReciprocalHeight,
				-(ViewLeft + ViewRight) * ReciprocalWidth
			);
			vValues = _mm_and_ps(vValues, g_MaskZ.v);
			M.v[3] = vValues;
			return M;
#	else
			float TwoNearZ = NearZ + NearZ;
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = FarZ / (FarZ - NearZ);
			const vec Zero = vdupq_n_f32(0);

			matrix M;
			M.v[0] = vsetq_lane_f32(TwoNearZ * ReciprocalWidth, Zero.f, 0);
			M.v[1] = vsetq_lane_f32(TwoNearZ * ReciprocalHeight, Zero.f, 1);
			M.v[2] = Vector(-(ViewLeft + ViewRight) * ReciprocalWidth,
				-(ViewTop + ViewBottom) * ReciprocalHeight,
				fRange,
				1.0f);
			M.v[3] = vsetq_lane_f32(-fRange * NearZ, Zero.f, 2);
			return M;
#	endif
#endif
		}
		
		MATH_CALL(matrix) matrix::PerspectiveOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)
		{
			assert(NearZ > 0.f && FarZ > 0.f);

			assert(!(Abs(ViewRight - ViewLeft) <= 0.00001f));
			assert(!(Abs(ViewTop - ViewBottom) <= 0.00001f));
			assert(!(Abs(FarZ - NearZ) <= 0.00001f));
#ifndef MATH_USE_SIMD
			float TwoNearZ = NearZ + NearZ;
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = FarZ / (NearZ - FarZ);

			matrix M;
			M.v[0].f[0] = TwoNearZ * ReciprocalWidth;
			M.v[0].f[1] = 0.0f;
			M.v[0].f[2] = 0.0f;
			M.v[0].f[3] = 0.0f;

			M.v[1].f[0] = 0.0f;
			M.v[1].f[1] = TwoNearZ * ReciprocalHeight;
			M.v[1].f[2] = 0.0f;
			M.v[1].f[3] = 0.0f;

			M.v[2].f[0] = (ViewLeft + ViewRight) * ReciprocalWidth;
			M.v[2].f[1] = (ViewTop + ViewBottom) * ReciprocalHeight;
			M.v[2].f[2] = fRange;
			M.v[2].f[3] = -1.0f;

			M.v[3].f[0] = 0.0f;
			M.v[3].f[1] = 0.0f;
			M.v[3].f[2] = fRange * NearZ;
			M.v[3].f[3] = 0.0f;
			return M;
#else
#	if defined(SIMD_SSE2)
			matrix M;
			float TwoNearZ = NearZ + NearZ;
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = FarZ / (NearZ - FarZ);
			__m128 rMem = {
				TwoNearZ*ReciprocalWidth,
				TwoNearZ*ReciprocalHeight,
				fRange * NearZ,
				0
			};
			__m128 vValues = rMem;
			__m128 vTemp = _mm_setzero_ps();
			vTemp = _mm_move_ss(vTemp, vValues);
			M.v[0] = vTemp;
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_MaskY.v);
			M.v[1] = vTemp;
			M.v[2] = _mm_set_ps(-1.0f,
				fRange,
				(ViewTop + ViewBottom) * ReciprocalHeight,
				(ViewLeft + ViewRight) * ReciprocalWidth
			);
			vValues = _mm_and_ps(vValues, g_MaskZ.v);
			M.v[3] = vValues;
			return M;
#	else
			float TwoNearZ = NearZ + NearZ;
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = FarZ / (NearZ - FarZ);
			const vec Zero = vdupq_n_f32(0);

			matrix M;
			M.v[0] = vsetq_lane_f32(TwoNearZ * ReciprocalWidth, Zero.f, 0);
			M.v[1] = vsetq_lane_f32(TwoNearZ * ReciprocalHeight, Zero.f, 1);
			M.v[2] = Vector((ViewLeft + ViewRight) * ReciprocalWidth,
				(ViewTop + ViewBottom) * ReciprocalHeight,
				fRange,
				-1.0f);
			M.v[3] = vsetq_lane_f32(fRange * NearZ, Zero.f, 2);
			return M;
#	endif
#endif
		}
		
		MATH_CALL(matrix) matrix::OrthographicLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
		{
			assert(!(Abs(ViewWidth - 0.0f) <= 0.00001f));
			assert(!(Abs(ViewHeight - 0.0f) <= 0.00001f));
			assert(!(Abs(FarZ - NearZ) <= 0.00001f));

#ifndef MATH_USE_SIMD
			float fRange = 1.0f / (FarZ - NearZ);

			matrix M;
			M.v[0].f[0] = 2.0f / ViewWidth;
			M.v[0].f[1] = 0.0f;
			M.v[0].f[2] = 0.0f;
			M.v[0].f[3] = 0.0f;

			M.v[1].f[0] = 0.0f;
			M.v[1].f[1] = 2.0f / ViewHeight;
			M.v[1].f[2] = 0.0f;
			M.v[1].f[3] = 0.0f;

			M.v[2].f[0] = 0.0f;
			M.v[2].f[1] = 0.0f;
			M.v[2].f[2] = fRange;
			M.v[2].f[3] = 0.0f;

			M.v[3].f[0] = 0.0f;
			M.v[3].f[1] = 0.0f;
			M.v[3].f[2] = -fRange * NearZ;
			M.v[3].f[3] = 1.0f;
			return M;
#else
#	if defined(SIMD_SSE2)
			matrix M;
			float fRange = 1.0f / (FarZ - NearZ);
			__m128 rMem = {
				2.0f / ViewWidth,
				2.0f / ViewHeight,
				fRange,
				-fRange * NearZ
			};
			__m128 vValues = rMem;
			__m128 vTemp = _mm_setzero_ps();
			vTemp = _mm_move_ss(vTemp, vValues);
			M.v[0] = vTemp;
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_MaskY.v);
			M.v[1] = vTemp;
			vTemp = _mm_setzero_ps();
			vValues = _mm_shuffle_ps(vValues, g_IdentityR3.v, _MM_SHUFFLE(3, 2, 3, 2));
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 0, 0, 0));
			M.v[2] = vTemp;
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 1, 0, 0));
			M.v[3] = vTemp;
			return M;
#	else
			float fRange = 1.0f / (FarZ - NearZ);

			const vec Zero = vdupq_n_f32(0);
			matrix M;
			M.v[0] = vsetq_lane_f32(2.0f / ViewWidth, Zero.f, 0);
			M.v[1] = vsetq_lane_f32(2.0f / ViewHeight, Zero.f, 1);
			M.v[2] = vsetq_lane_f32(fRange, Zero.f, 2);
			M.v[3] = vsetq_lane_f32(-fRange * NearZ, g_IdentityR3.v.f, 2);
			return M;
#	endif
#endif
		}

		MATH_CALL(matrix) matrix::OrthographicRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
		{
			assert(!(Abs(ViewWidth - 0.0f) <= 0.00001f));
			assert(!(Abs(ViewHeight - 0.0f) <= 0.00001f));
			assert(!(Abs(FarZ - NearZ) <= 0.00001f));

#ifndef MATH_USE_SIMD
			float fRange = 1.0f / (NearZ - FarZ);

			matrix M;
			M.v[0].f[0] = 2.0f / ViewWidth;
			M.v[0].f[1] = 0.0f;
			M.v[0].f[2] = 0.0f;
			M.v[0].f[3] = 0.0f;

			M.v[1].f[0] = 0.0f;
			M.v[1].f[1] = 2.0f / ViewHeight;
			M.v[1].f[2] = 0.0f;
			M.v[1].f[3] = 0.0f;

			M.v[2].f[0] = 0.0f;
			M.v[2].f[1] = 0.0f;
			M.v[2].f[2] = fRange;
			M.v[2].f[3] = 0.0f;

			M.v[3].f[0] = 0.0f;
			M.v[3].f[1] = 0.0f;
			M.v[3].f[2] = fRange * NearZ;
			M.v[3].f[3] = 1.0f;
			return M;
#else
#	if defined(SIMD_SSE2)
			matrix M;
			float fRange = 1.0f / (NearZ - FarZ);
			__m128 rMem = {
				2.0f / ViewWidth,
				2.0f / ViewHeight,
				fRange,
				fRange * NearZ
			};
			__m128 vValues = rMem;
			__m128 vTemp = _mm_setzero_ps();
			vTemp = _mm_move_ss(vTemp, vValues);
			M.v[0] = vTemp;
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_MaskY.v);
			M.v[1] = vTemp;
			vTemp = _mm_setzero_ps();
			vValues = _mm_shuffle_ps(vValues, g_IdentityR3.v, _MM_SHUFFLE(3, 2, 3, 2));
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 0, 0, 0));
			M.v[2] = vTemp;
			vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 1, 0, 0));
			M.v[3] = vTemp;
			return M;
#	else
			float fRange = 1.0f / (NearZ - FarZ);

			const vec Zero = vdupq_n_f32(0);
			matrix M;
			M.v[0] = vsetq_lane_f32(2.0f / ViewWidth, Zero.f, 0);
			M.v[1] = vsetq_lane_f32(2.0f / ViewHeight, Zero.f, 1);
			M.v[2] = vsetq_lane_f32(fRange, Zero.f, 2);
			M.v[3] = vsetq_lane_f32(fRange * NearZ, g_IdentityR3.v.f, 2);
			return M;
#	endif
#endif
		}

		MATH_CALL(matrix) matrix::OrthographicOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)
		{
			assert(!(Abs(ViewRight - ViewLeft) <= 0.00001f));
			assert(!(Abs(ViewTop - ViewBottom) <= 0.00001f));
			assert(!(Abs(FarZ - NearZ) <= 0.00001f));

#ifndef MATH_USE_SIMD
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = 1.0f / (FarZ - NearZ);

			matrix M;
			M.v[0].f[0] = ReciprocalWidth + ReciprocalWidth;
			M.v[0].f[1] = 0.0f;
			M.v[0].f[2] = 0.0f;
			M.v[0].f[3] = 0.0f;

			M.v[1].f[0] = 0.0f;
			M.v[1].f[1] = ReciprocalHeight + ReciprocalHeight;
			M.v[1].f[2] = 0.0f;
			M.v[1].f[3] = 0.0f;

			M.v[2].f[0] = 0.0f;
			M.v[2].f[1] = 0.0f;
			M.v[2].f[2] = fRange;
			M.v[2].f[3] = 0.0f;

			M.v[3].f[0] = -(ViewLeft + ViewRight) * ReciprocalWidth;
			M.v[3].f[1] = -(ViewTop + ViewBottom) * ReciprocalHeight;
			M.v[3].f[2] = -fRange * NearZ;
			M.v[3].f[3] = 1.0f;
			return M;
#else
#	if defined(SIMD_SSE2)
			matrix M;
			float fReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float fReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = 1.0f / (FarZ - NearZ);
			__m128 rMem = {
				fReciprocalWidth,
				fReciprocalHeight,
				fRange,
				1.0f
			};
			__m128 rMem2 = {
				-(ViewLeft + ViewRight),
				-(ViewTop + ViewBottom),
				-NearZ,
				1.0f
			};
			__m128 vValues = rMem;
			__m128 vTemp = _mm_setzero_ps();
			vTemp = _mm_move_ss(vTemp, vValues);
			vTemp = _mm_add_ss(vTemp, vTemp);
			M.v[0] = vTemp;
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_MaskY.v);
			vTemp = _mm_add_ps(vTemp, vTemp);
			M.v[1] = vTemp;
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_MaskZ.v);
			M.v[2] = vTemp;
			vValues = _mm_mul_ps(vValues, rMem2);
			M.v[3] = vValues;
			return M;
#	else
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = 1.0f / (FarZ - NearZ);
			const vec Zero = vdupq_n_f32(0);
			matrix M;
			M.v[0] = vsetq_lane_f32(ReciprocalWidth + ReciprocalWidth, Zero.f, 0);
			M.v[1] = vsetq_lane_f32(ReciprocalHeight + ReciprocalHeight, Zero.f, 1);
			M.v[2] = vsetq_lane_f32(fRange, Zero.f, 2);
			M.v[3] = Vector(-(ViewLeft + ViewRight) * ReciprocalWidth,
				-(ViewTop + ViewBottom) * ReciprocalHeight,
				-fRange * NearZ,
				1.0f);
			return M;
#	endif
#endif
		}

		MATH_CALL(matrix) matrix::OrthographicOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)
		{
			assert(!(Abs(ViewRight - ViewLeft) <= 0.00001f));
			assert(!(Abs(ViewTop - ViewBottom) <= 0.00001f));
			assert(!(Abs(FarZ - NearZ) <= 0.00001f));
#ifndef MATH_USE_SIMD
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = 1.0f / (NearZ - FarZ);

			matrix M;
			M.v[0].f[0] = ReciprocalWidth + ReciprocalWidth;
			M.v[0].f[1] = 0.0f;
			M.v[0].f[2] = 0.0f;
			M.v[0].f[3] = 0.0f;

			M.v[1].f[0] = 0.0f;
			M.v[1].f[1] = ReciprocalHeight + ReciprocalHeight;
			M.v[1].f[2] = 0.0f;
			M.v[1].f[3] = 0.0f;

			M.v[2].f[0] = 0.0f;
			M.v[2].f[1] = 0.0f;
			M.v[2].f[2] = fRange;
			M.v[2].f[3] = 0.0f;

			M.v[3].f[0] = -(ViewLeft + ViewRight) * ReciprocalWidth;
			M.v[3].f[0] = -(ViewTop + ViewBottom) * ReciprocalHeight;
			M.v[3].f[0] = fRange * NearZ;
			M.v[3].f[0] = 1.0f;
			return M;
#else
#	if defined(SIMD_SSE2)
			matrix M;
			float fReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float fReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = 1.0f / (NearZ - FarZ);
			__m128 rMem = {
				fReciprocalWidth,
				fReciprocalHeight,
				fRange,
				1.0f
			};
			__m128 rMem2 = {
				-(ViewLeft + ViewRight),
				-(ViewTop + ViewBottom),
				NearZ,
				1.0f
			};
			__m128 vValues = rMem;
			__m128 vTemp = _mm_setzero_ps();
			vTemp = _mm_move_ss(vTemp, vValues);
			vTemp = _mm_add_ss(vTemp, vTemp);
			M.v[0] = vTemp;
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_MaskY.v);
			vTemp = _mm_add_ps(vTemp, vTemp);
			M.v[1] = vTemp;
			vTemp = vValues;
			vTemp = _mm_and_ps(vTemp, g_MaskZ.v);
			M.v[2] = vTemp;
			vValues = _mm_mul_ps(vValues, rMem2);
			M.v[3] = vValues;
			return M;
#	else
			float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
			float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
			float fRange = 1.0f / (NearZ - FarZ);
			const vec Zero = vdupq_n_f32(0);
			matrix M;
			M.v[0] = vsetq_lane_f32(ReciprocalWidth + ReciprocalWidth, Zero.f, 0);
			M.v[1] = vsetq_lane_f32(ReciprocalHeight + ReciprocalHeight, Zero.f, 1);
			M.v[2] = vsetq_lane_f32(fRange, Zero.f, 2);
			M.v[3] = Vector(-(ViewLeft + ViewRight) * ReciprocalWidth,
				-(ViewTop + ViewBottom) * ReciprocalHeight,
				fRange * NearZ,
				1.0f);
			return M;
#	endif
#endif
		}

		matrix::matrix()
		{
		}

		matrix::matrix(const matrix& _m)
		{
			this->v[0] = _m.v[0];
			this->v[1] = _m.v[1];
			this->v[2] = _m.v[2];
			this->v[3] = _m.v[3];
		}

		matrix::matrix(const float* f)
		{
			memcpy(this->v, f, sizeof(float4) * 4);
		}

		MATH_CALL(float) matrix::operator()(uint32_t x, uint32_t y) const
		{
			const float* f = reinterpret_cast<const float*>(this->v);
			return f[x * 4 + y];
		}

		MATH_CALL(float&) matrix::operator()(uint32_t x, uint32_t y)
		{
			float* f = reinterpret_cast<float*>(this->v);
			return f[x * 4 + y];
		}

		MATH_CALL(vec&) matrix::operator[](int i) { return this->v[i]; }
		MATH_CALL(vec) matrix::operator[](int i) const { return this->v[i]; }

		std::ostream& operator<<(std::ostream& stream, const matrix& m)
		{
			const float* f = reinterpret_cast<const float*>(&m);
			stream << '|' << f[0] << ' ' << f[1] << ' ' << f[2] << ' ' << f[3] << '|' << std::endl;
			stream << '|' << f[4] << ' ' << f[5] << ' ' << f[6] << ' ' << f[7] << '|' << std::endl;
			stream << '|' << f[8] << ' ' << f[9] << ' ' << f[10] << ' ' << f[11] << '|' << std::endl;
			stream << '|' << f[12] << ' ' << f[13] << ' ' << f[14] << ' ' << f[15] << '|' << std::endl;
			return stream;
		}

		std::ostream& operator<<(std::ostream& stream, const float4& f)
		{
			stream << '(' << f.x << ',' << f.y << ',' << f.z << ',' << f.w << ')';
			return stream;
		}

		std::ostream& operator<<(std::ostream& stream, const float3& f)
		{
			stream << '(' << f.x << ',' << f.y << ',' << f.z << ')';
			return stream;
		}

		std::ostream& operator<<(std::ostream& stream, const float2& f)
		{
			stream << '(' << f.x << ',' << f.y << ')';
			return stream;
		}
		
		std::ostream& operator<<(std::ostream& stream, const int2& f)
		{
			stream << '(' << f.x << ',' << f.y << ')';
			return stream;
		}

		std::ostream& operator<<(std::ostream& stream, const int4& f)
		{
			stream << '(' << f.x << ',' << f.y << ',' << f.z << ',' << f.w << ')';
			return stream;
		}

		std::ostream& operator<<(std::ostream& stream, const int3& f)
		{
			stream << '(' << f.x << ',' << f.y << ',' << f.z << ')';
			return stream;
		}

		std::ostream& operator<<(std::ostream& stream, const uint2& f)
		{
			stream << '(' << f.x << ',' << f.y << ')';
			return stream;
		}

		std::ostream& operator<<(std::ostream& stream, const uint4& f)
		{
			stream << '(' << f.x << ',' << f.y << ',' << f.z << ',' << f.w << ')';
			return stream;
		}

		std::ostream& operator<<(std::ostream& stream, const uint3& f)
		{
			//std::ios::fmtflags oldflag = stream.setf(std::ios::fixed, std::ios::floatfield);
			//std::streamsize oldprecision = stream.precision(6);
			stream << '(' << f.x << ',' << f.y << ',' << f.z << ')';
			//stream.setf(oldflag, std::ios::floatfield);
			//stream.precision(oldprecision);
			return stream;
		}

		MATH_CALL(vec) load(const float4& v)
		{
			return LoadVec<4>(reinterpret_cast<const float*>(&v));
		}

		MATH_CALL(vec) load(const float3& v)
		{
			return LoadVec<3>(reinterpret_cast<const float*>(&v));
		}

		MATH_CALL(vec) load(const float2& v)
		{
			return LoadVec<2>(reinterpret_cast<const float*>(&v));
		}


		MATH_CALL(void) store(vec v, float4& f)
		{
			StoreVec<4>(reinterpret_cast<float*>(&f), v);
		}

		MATH_CALL(void) store(vec v, float3& f)
		{
			StoreVec<3>(reinterpret_cast<float*>(&f), v);
		}

		MATH_CALL(void) store(vec v, float2& f)
		{
			StoreVec<3>(reinterpret_cast<float*>(&f), v);
		}

		MATH_CALL(vec) normalize4(vec_f v)
		{
			return NormalizeVec<4>(v);
		}

		MATH_CALL(vec) normalize3(vec_f v)
		{
			return NormalizeVec<3>(v);
		}

		MATH_CALL(vec) normalize2(vec_f v)
		{
			return NormalizeVec<3>(v);
		}

		MATH_CALL(vec) sqrt(vec_f v)
		{
			return SqrtVec(v);
		}

		MATH_CALL(vec) inversesqrt(vec_f v)
		{
			return InverseSqrtVec(v);
		}

		MATH_CALL(float) length4(vec_f v)
		{
			return LengthVec<4>(v);
		}

		MATH_CALL(float) length3(vec_f v)
		{
			return LengthVec<3>(v);
		}

		MATH_CALL(float) length2(vec_f v)
		{
			return LengthVec<2>(v);
		}

		MATH_CALL(vec) dot4(vec_f a, vec_f b)
		{
			return DotVec<4>(a, b);
		}

		MATH_CALL(vec) dot3(vec_f a, vec_f b)
		{
			return DotVec<3>(a, b);
		}

		MATH_CALL(vec) dot2(vec_f a, vec_f b)
		{
			return DotVec<2>(a, b);
		}

		MATH_CALL(float4) cross(const float4& vec1, const float4& vec2, const float4& vec3)
		{
			float4 ret;
			StoreVec<4>(reinterpret_cast<float*>(&ret), CrossVec4(
				LoadVec<4>(reinterpret_cast<const float*>(&vec1)),
				LoadVec<4>(reinterpret_cast<const float*>(&vec2)),
				LoadVec<4>(reinterpret_cast<const float*>(&vec3))
				));
			return ret;
		}

		MATH_CALL(vec) cross4(vec_f a, vec_f b, vec_f c)
		{
			return CrossVec4(a, b, c);
		}

		MATH_CALL(vec) cross3(vec_f a, vec_f b)
		{
			return CrossVec<3>(a, b);
		}

		MATH_CALL(vec) cross2(vec_f a, vec_f b)
		{
			return CrossVec<2>(a, b);
		}

		MATH_CALL(float3) cross(const float3& vec1, const float3& vec2)
		{
			float3 ret;
			StoreVec<3>(reinterpret_cast<float*>(&ret), CrossVec<3>(
					LoadVec<3>(reinterpret_cast<const float*>(&vec1)),
					LoadVec<3>(reinterpret_cast<const float*>(&vec2))
				));
			return ret;
		}

		MATH_CALL(float2) cross(const float2& vec1, const float2& vec2)
		{
			float2 ret;
			StoreVec<2>(reinterpret_cast<float*>(&ret), CrossVec<2>(
				LoadVec<2>(reinterpret_cast<const float*>(&vec1)),
				LoadVec<2>(reinterpret_cast<const float*>(&vec2))
				));
			return ret;
		}

		MATH_CALL(vec) set(float x, float y, float z, float w)
		{
#ifndef MATH_USE_SIMD
			return { x,y,z,w };
#else
#	if defined(SIMD_SSE2)
			return _mm_set_ps(w, z, y, x);
#	else
			float32x2_t V0 = vcreate_f32(((uint64_t)*(const uint32_t *)&x) | ((uint64_t)(*(const uint32_t *)&y) << 32));
			float32x2_t V1 = vcreate_f32(((uint64_t)*(const uint32_t *)&z) | ((uint64_t)(*(const uint32_t *)&w) << 32));
			return vcombine_f32(V0, V1);
#	endif
#endif
		}

		MATH_CALL(vec) operator-(vec_f a, vec_f b)
		{
			return SubVec(a, b);
		}

		MATH_CALL(vec) operator+(vec_f a, vec_f b)
		{
			return AddVec(a, b);
		}

		MATH_CALL(vec) operator*(vec_f a, vec_f b)
		{
			return MultiplyVec(a, b);
		}

		MATH_CALL(vec) operator/(vec_f a, vec_f b)
		{
#ifndef MATH_USE_SIMD
			vec Result;
			Result.f[0] = a.f[0] / b.f[0];
			Result.f[1] = a.f[1] / b.f[1];
			Result.f[2] = a.f[2] / b.f[2];
			Result.f[3] = a.f[3] / b.f[3];
			return Result;
#else
#	if defined(SIMD_SSE2)
			return _mm_div_ps(a, b);
#	else
			float32x4_t Reciprocal = vrecpeq_f32(b.f);
			float32x4_t S = vrecpsq_f32(Reciprocal, b.f);
			Reciprocal = vmulq_f32(S, Reciprocal);
			S = vrecpsq_f32(Reciprocal, b.f);
			Reciprocal = vmulq_f32(S, Reciprocal);
			return vmulq_f32(a.f, Reciprocal);
#	endif
#endif
		}

		MATH_CALL(vec) reciprocal(vec_f v)
		{
#ifndef MATH_USE_SIMD
			vec Result;
			Result.f[0] = 1.0f / v.f[0];
			Result.f[1] = 1.0f / v.f[1];
			Result.f[2] = 1.0f / v.f[2];
			Result.f[3] = 1.0f / v.f[3];
			return Result;
#else
#	if defined(SIMD_SSE2)
			return _mm_div_ps(g_One.v, v);
#	else
			float32x4_t Reciprocal = vrecpeq_f32(v.f);
			float32x4_t S = vrecpsq_f32(Reciprocal, v.f);
			Reciprocal = vmulq_f32(S, Reciprocal);
			S = vrecpsq_f32(Reciprocal, v.f);
			return vmulq_f32(S, Reciprocal);
#	endif
#endif
		}

		MATH_CALL(vec) sin(vec_f v)
		{
#ifndef MATH_USE_SIMD
			vec ret;
			ret.f[0] = ::sinf(v.f[0]);
			ret.f[1] = ::sinf(v.f[1]);
			ret.f[2] = ::sinf(v.f[2]);
			ret.f[3] = ::sinf(v.f[3]);
			return ret;
#else
#	if defined(SIMD_SSE2)
			__m128 x = _mm_mul_ps(v, g_ReciprocalTwoPi.v);
			__m128 _sign = _mm_and_ps(x, g_NegativeZero.v);
			__m128 _sMagic = _mm_or_ps(g_NoFraction.v, _sign);
			__m128 _R1 = _mm_add_ps(x, _sMagic);
			_R1 = _mm_sub_ps(_R1, _sMagic);
			__m128 _R2 = _mm_and_ps(x, g_AbsMask.v);
			__m128 _mask = _mm_cmple_ps(_R2, g_NoFraction.v);
			_R2 = _mm_andnot_ps(_mask, x);
			_R1 = _mm_and_ps(_R1, _mask);
			x = _mm_xor_ps(_R1, _R2);
			x = _mm_mul_ps(x, g_TwoPi.v);
			x = _mm_sub_ps(v, x);
			__m128 sign = _mm_and_ps(x, g_NegativeZero.v);
			__m128 c = _mm_or_ps(g_Pi.v, sign);  // pi when x >= 0, -pi when x < 0
			__m128 absx = _mm_andnot_ps(sign, x);  // |x|
			__m128 rflx = _mm_sub_ps(c, x);
			__m128 comp = _mm_cmple_ps(absx, g_HalfPi.v);
			__m128 select0 = _mm_and_ps(comp, x);
			__m128 select1 = _mm_andnot_ps(comp, rflx);
			x = _mm_or_ps(select0, select1);

			__m128 x2 = _mm_mul_ps(x, x);

			// Compute polynomial approximation
			const __m128 SC1 = g_SinCoefficients1.v;
			__m128 vConstants = _mm_shuffle_ps(SC1, SC1, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 Result = _mm_mul_ps(vConstants, x2);

			const __m128 SC0 = g_SinCoefficients0.v;
			vConstants = _mm_shuffle_ps(SC0, SC0, _MM_SHUFFLE(3, 3, 3, 3));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = _mm_shuffle_ps(SC0, SC0, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = _mm_shuffle_ps(SC0, SC0, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = _mm_shuffle_ps(SC0, SC0, _MM_SHUFFLE(0, 0, 0, 0));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);
			Result = _mm_add_ps(Result, g_One.v);
			Result = _mm_mul_ps(Result, x);
			return Result;
#	else
			vec x = vmulq_f32(v.f, g_ReciprocalTwoPi.v.f);
			x = round(x);
			x = vmlsq_f32(v.f, x.f, g_TwoPi.v.f);

			// Map in [-pi/2,pi/2] with sin(y) = sin(x).
			uint32x4_t sign = vandq_u32(x.u, g_NegativeZero.v.u);
			uint32x4_t c = vorrq_u32(g_Pi.v.u, sign);  // pi when x >= 0, -pi when x < 0
			float32x4_t absx = vabsq_f32(x.f);
			float32x4_t rflx = vsubq_f32(vec(c).f, x.f);
			uint32x4_t comp = vcleq_f32(absx, g_HalfPi.v.f);
			x = vbslq_f32(comp, x.f, rflx);

			float32x4_t x2 = vmulq_f32(x.f, x.f);

			// Compute polynomial approximation
			const vec SC1 = g_SinCoefficients1.v;
			const vec SC0 = g_SinCoefficients0.v;
			vec vConstants = vdupq_lane_f32(vget_high_f32(SC0.f), 1);
			vec Result = vmlaq_lane_f32(vConstants.f, x2, vget_low_f32(SC1.f), 0);

			vConstants = vdupq_lane_f32(vget_high_f32(SC0.f), 0);
			Result = vmlaq_f32(vConstants.f, Result.f, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(SC0.f), 1);
			Result = vmlaq_f32(vConstants.f, Result.f, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(SC0.f), 0);
			Result = vmlaq_f32(vConstants.f, Result.f, x2);

			Result = vmlaq_f32(g_One.v.f, Result.f, x2);
			Result = vmulq_f32(Result.f, x.f);
			return Result;
#	endif
#endif
		}

		MATH_CALL(vec) cos(vec_f v)
		{
#ifndef MATH_USE_SIMD
			vec ret;
			ret.f[0] = ::cosf(v.f[0]);
			ret.f[1] = ::cosf(v.f[1]);
			ret.f[2] = ::cosf(v.f[2]);
			ret.f[3] = ::cosf(v.f[3]);
			return ret;
#else
#	if defined(SIMD_SSE2)
			__m128 x = _mm_mul_ps(v, g_ReciprocalTwoPi.v);

			__m128 sign = _mm_and_ps(x, g_NegativeZero.v);
			__m128 sMagic = _mm_or_ps(g_NoFraction.v, sign);
			__m128 R1 = _mm_add_ps(x, sMagic);
			R1 = _mm_sub_ps(R1, sMagic);
			__m128 R2 = _mm_and_ps(x, g_AbsMask.v);
			__m128 mask = _mm_cmple_ps(R2, g_NoFraction.v);
			R2 = _mm_andnot_ps(mask, x);
			R1 = _mm_and_ps(R1, mask);
			x = _mm_xor_ps(R1, R2);

			x = _mm_mul_ps(x, g_TwoPi.v);
			x = _mm_sub_ps(v, x);

			// Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
			sign = _mm_and_ps(x, g_NegativeZero.v);
			__m128 c = _mm_or_ps(g_Pi.v, sign);  // pi when x >= 0, -pi when x < 0
			__m128 absx = _mm_andnot_ps(sign, x);  // |x|
			__m128 rflx = _mm_sub_ps(c, x);
			__m128 comp = _mm_cmple_ps(absx, g_HalfPi.v);
			__m128 select0 = _mm_and_ps(comp, x);
			__m128 select1 = _mm_andnot_ps(comp, rflx);
			x = _mm_or_ps(select0, select1);
			select0 = _mm_and_ps(comp, g_One.v);
			select1 = _mm_andnot_ps(comp, g_NegativeOne.v);
			sign = _mm_or_ps(select0, select1);

			__m128 x2 = _mm_mul_ps(x, x);

			// Compute polynomial approximation
			const __m128 CC1 = g_CosCoefficients1.v;
			__m128 vConstants = _mm_shuffle_ps(CC1, CC1, _MM_SHUFFLE(0, 0, 0, 0));
			__m128 Result = _mm_mul_ps(vConstants, x2);

			const __m128 CC0 = g_CosCoefficients0.v;
			vConstants = _mm_shuffle_ps(CC0, CC0, _MM_SHUFFLE(3, 3, 3, 3));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = _mm_shuffle_ps(CC0, CC0, _MM_SHUFFLE(2, 2, 2, 2));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = _mm_shuffle_ps(CC0, CC0, _MM_SHUFFLE(1, 1, 1, 1));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);

			vConstants = _mm_shuffle_ps(CC0, CC0, _MM_SHUFFLE(0, 0, 0, 0));
			Result = _mm_add_ps(Result, vConstants);
			Result = _mm_mul_ps(Result, x2);
			Result = _mm_add_ps(Result, g_One.v);
			Result = _mm_mul_ps(Result, sign);
			return Result;
#	else
			vec x = vmulq_f32(v.f, g_ReciprocalTwoPi.v.f);
			x = round(x);
			x = vmlsq_f32(v.f, x.f, g_TwoPi.v.f);

			// Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
			vec sign = vandq_u32(x.u, g_NegativeZero.v.u);
			uint32x4_t c = vorrq_u32(g_Pi.v.u, sign.u);  // pi when x >= 0, -pi when x < 0
			float32x4_t absx = vabsq_f32(x.f);
			float32x4_t rflx = vsubq_f32(vec(c).f, x.f);
			uint32x4_t comp = vcleq_f32(absx, g_HalfPi.v.f);
			x = vbslq_f32(comp, x.f, rflx);
			sign = vbslq_f32(comp, g_One.v.f, g_NegativeOne.v.f);

			float32x4_t x2 = vmulq_f32(x.f, x.f);

			// Compute polynomial approximation
			const vec CC1 = g_CosCoefficients1.v;
			const vec CC0 = g_CosCoefficients0.v;
			vec vConstants = vdupq_lane_f32(vget_high_f32(CC0.f), 1);
			vec Result = vmlaq_lane_f32(vConstants.f, x2, vget_low_f32(CC1.f), 0);

			vConstants = vdupq_lane_f32(vget_high_f32(CC0.f), 0);
			Result = vmlaq_f32(vConstants.f, Result.f, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(CC0.f), 1);
			Result = vmlaq_f32(vConstants.f, Result.f, x2);

			vConstants = vdupq_lane_f32(vget_low_f32(CC0.f), 0);
			Result = vmlaq_f32(vConstants.f, Result.f, x2);

			Result = vmlaq_f32(g_One.v.f, Result.f, x2);
			Result = vmulq_f32(Result.f, sign.f);
			return Result;
#	endif
#endif
		}

		MATH_CALL(float) get(vec_f v, uint32_t index)
		{
			union _fVec
			{
				float i[4];
				vec v;
				_fVec(vec_f v) :v(v) {}
			};
			const _fVec _v = v;
			return _v.i[0];
		}

		MATH_CALL(vec) round(vec_f v)
		{
#ifndef MATH_USE_SIMD
			vec ret;
			ret.f[0] = roundf(v.f[0]);
			ret.f[1] = roundf(v.f[1]);
			ret.f[2] = roundf(v.f[2]);
			ret.f[3] = roundf(v.f[3]);
			return ret;
#else
#	if defined(SIMD_SSE2)
			__m128 sign = _mm_and_ps(v, g_NegativeZero.v);
			__m128 sMagic = _mm_or_ps(g_NoFraction.v, sign);
			__m128 R1 = _mm_add_ps(v, sMagic);
			R1 = _mm_sub_ps(R1, sMagic);
			__m128 R2 = _mm_and_ps(v, g_AbsMask.v);
			__m128 mask = _mm_cmple_ps(R2, g_NoFraction.v);
			R2 = _mm_andnot_ps(mask, v);
			R1 = _mm_and_ps(R1, mask);
			__m128 vResult = _mm_xor_ps(R1, R2);
			return vResult;
#	else
			uint32x4_t sign = vandq_u32(v.u, g_NegativeZero.v.u);
			uint32x4_t sMagic = vorrq_u32(g_NoFraction.v.u, sign);
			float32x4_t R1 = vaddq_f32(v.f, vec(sMagic).f);
			R1 = vsubq_f32(R1, vec(sMagic).f);
			float32x4_t R2 = vabsq_f32(v.f);
			uint32x4_t mask = vcleq_f32(R2, g_NoFraction.v.f);
			vec vResult = vbslq_f32(mask, R1, v.f);
			return vResult;
#	endif
#endif
		}

		MATH_CALL(vec) tan(vec_f v);
		MATH_CALL(vec) asin(vec_f v);
		MATH_CALL(vec) acos(vec_f v);
		MATH_CALL(vec) atan(vec_f v);
		}

/*
#ifndef MATH_USE_SIMD
#else
#	if defined(SIMD_SSE2)
#	else
#	endif
#endif
*/
#pragma once

#include <stdint.h>
#include <Skuld/MathStruct.h>
#include <ostream>

#if defined (_MSC_VER)
#	define MATH_CALL(type) type __fastcall
#	define MATH_ALIGN(n) __declspec(align(n))
#elif defined (__GNUC__)
#	if defined(__arm__) || defined(__aarch64__)
#		define MATH_CALL(type) type
#	else
#		define MATH_CALL(type) __attribute__((regparm(3))) type
#	endif
#	define MATH_ALIGN(n) __attribute__((aligned(n)))
#endif

#if defined(SIMD_NEON)
#include <arm_neon.h>
#endif

#if defined(SIMD_SSE2)
#include <xmmintrin.h>
#endif

namespace Skuld
{
	typedef float4 vec; ///TODO: 此处应替换成SIMD的Vector类型
	typedef const vec vec_f;

	std::ostream& operator<<(std::ostream& stream, const float2& f);
	std::ostream& operator<<(std::ostream& stream, const float4& f);
	std::ostream& operator<<(std::ostream& stream, const float3& f);

	std::ostream& operator<<(std::ostream& stream, const int2& f);
	std::ostream& operator<<(std::ostream& stream, const int4& f);
	std::ostream& operator<<(std::ostream& stream, const int3& f);

	std::ostream& operator<<(std::ostream& stream, const uint2& f);
	std::ostream& operator<<(std::ostream& stream, const uint4& f);
	std::ostream& operator<<(std::ostream& stream, const uint3& f);
	
	class MATH_ALIGN(16) matrix
	{
	private:
		union
		{
			float f[16];
			struct
			{
				float f[4];
			} fa[4];
			vec v[4];
		};
	public:
		matrix();
		matrix(const float* f);
		matrix(const matrix&);
		MATH_CALL(float) operator()(uint32_t x, uint32_t y) const;
		MATH_CALL(float&) operator()(uint32_t x, uint32_t y);
		MATH_CALL(vec&) operator[](int index);
		MATH_CALL(vec) operator[](int index) const;
		static MATH_CALL(matrix) Identity();
		static MATH_CALL(matrix) LookAtLH(const float4& eye_position, const float4& focus_position, const float4& up_direction);
		static MATH_CALL(matrix) LookAtRH(const float4& eye_position, const float4& focus_position, const float4& up_direction);
		static MATH_CALL(matrix) LookToLH(const float4& eye_position, const float4& eye_direction, const float4& up_direction);
		static MATH_CALL(matrix) LookToRH(const float4& eye_position, const float4& eye_direction, const float4& up_direction);
		static MATH_CALL(matrix) PerspectiveLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
		static MATH_CALL(matrix) PerspectiveRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
		static MATH_CALL(matrix) PerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ);
		static MATH_CALL(matrix) PerspectiveFovRH(float FovAngleY, float AspectRatio, float NearZ, float FarZ);
		static MATH_CALL(matrix) PerspectiveOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ);
		static MATH_CALL(matrix) PerspectiveOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ);
		static MATH_CALL(matrix) OrthographicLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
		static MATH_CALL(matrix) OrthographicRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
		static MATH_CALL(matrix) OrthographicOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ);
		static MATH_CALL(matrix) OrthographicOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ);
		static MATH_CALL(matrix) RotationAxis(const float4& axis, float angle);
		static MATH_CALL(matrix) RotationAxis(const float3& axis, float angle);
		static MATH_CALL(matrix) RotationAxis(vec_f axis, float angle);
		static MATH_CALL(matrix) Scaling(float x, float y, float z);
		static MATH_CALL(matrix) Translation(float x, float y, float z);
		static MATH_CALL(matrix) RotationX(float x);
		static MATH_CALL(matrix) RotationY(float y);
		static MATH_CALL(matrix) RotationZ(float z);
		MATH_CALL(matrix) InverseMatrix() const;
		MATH_CALL(matrix) TransposeMatrix() const;
		void Inverse();
		void Transpose();
		bool IsIdentity() const;
		bool IsInfinite() const;
		bool IsNaN() const;
		MATH_CALL(matrix&) operator=(const matrix& a);
		friend MATH_CALL(matrix) operator*(const matrix& a, float b);
		friend MATH_CALL(matrix) operator/(const matrix& a, float b);
		friend MATH_CALL(matrix) operator+(const matrix& a, const matrix& b);
		friend MATH_CALL(matrix) operator-(const matrix& a, const matrix& b);
		friend MATH_CALL(matrix) operator*(const matrix& a, const matrix& b);
		MATH_CALL(matrix&) operator*=(const matrix& a);
		MATH_CALL(matrix&) operator*=(float a);
		MATH_CALL(matrix&) operator+=(const matrix& a);
		MATH_CALL(matrix&) operator-=(const matrix& a);
		MATH_CALL(matrix&) operator/=(float a);
		friend std::ostream& operator<<(std::ostream& stream, const matrix& m);
	};

	MATH_CALL(vec) load(const float4& v);
	MATH_CALL(vec) load(const float3& v);
	MATH_CALL(vec) load(const float2& v);

	MATH_CALL(void) store(vec v, float4& f);
	MATH_CALL(void) store(vec v, float3& f);
	MATH_CALL(void) store(vec v, float2& f);

	MATH_CALL(float4) normalize(const float4& v);
	MATH_CALL(float3) normalize(const float3& v);
	MATH_CALL(float2) normalize(const float2& v);

	MATH_CALL(float4) dot(const float4& vec1, const float4& vec2);
	MATH_CALL(float3) dot(const float3& vec1, const float3& vec2);
	MATH_CALL(float2) dot(const float2& vec1, const float2& vec2);

	MATH_CALL(float4) sqrt(const float4& v);
	MATH_CALL(float3) sqrt(const float3& v);
	MATH_CALL(float2) sqrt(const float2& v);

	MATH_CALL(float4) inversesqrt(const float4& v);
	MATH_CALL(float3) inversesqrt(const float3& v);
	MATH_CALL(float2) inversesqrt(const float2& v);

	MATH_CALL(float) length(const float4& v);
	MATH_CALL(float) length(const float3& v);
	MATH_CALL(float) length(const float2& v);

	MATH_CALL(vec) normalize4(vec_f v);
	MATH_CALL(vec) normalize3(vec_f v);
	MATH_CALL(vec) normalize2(vec_f v);
	MATH_CALL(vec) sqrt(vec_f v);
	MATH_CALL(vec) inversesqrt(vec_f v);
	MATH_CALL(float) length4(vec_f v);
	MATH_CALL(float) length3(vec_f v);
	MATH_CALL(float) length2(vec_f v);
	MATH_CALL(vec) dot4(vec_f a, vec_f b);
	MATH_CALL(vec) dot3(vec_f a, vec_f b);
	MATH_CALL(vec) dot2(vec_f a, vec_f b);

	MATH_CALL(float4) cross(const float4& vec1, const float4& vec2, const float4& vec3);
	MATH_CALL(float3) cross(const float3& vec1, const float3& vec2);
	MATH_CALL(float2) cross(const float2& vec1, const float2& vec2);
	MATH_CALL(vec) cross4(vec_f a, vec_f b, vec_f c);
	MATH_CALL(vec) cross3(vec_f a, vec_f b);
	MATH_CALL(vec) cross2(vec_f a, vec_f b);

	MATH_CALL(vec) set(float x, float y, float z, float w);
	MATH_CALL(vec) operator-(vec_f a, vec_f b);
	MATH_CALL(vec) operator+(vec_f a, vec_f b);
	MATH_CALL(vec) operator*(vec_f a, vec_f b);
	MATH_CALL(vec) operator/(vec_f a, vec_f b);

	MATH_CALL(vec) reciprocal(vec_f v);

	MATH_CALL(vec) sin(vec_f v);
	MATH_CALL(vec) cos(vec_f v);
	MATH_CALL(vec) tan(vec_f v);
	MATH_CALL(vec) asin(vec_f v);
	MATH_CALL(vec) acos(vec_f v);
	MATH_CALL(vec) atan(vec_f v);
	MATH_CALL(vec) round(vec_f v);

	MATH_CALL(float) get(vec_f v, uint32_t index);
}
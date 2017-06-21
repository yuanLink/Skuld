#pragma once

#include "SIMDMath.h"

namespace Skuld
{
	template<typename T> static inline T Min(const T a, const T b) { return (a > b ? b : a); }
	template<typename T> static inline T Max(const T a, const T b) { return (a < b ? b : a); }
	template<typename T> static inline T Abs(const T a) { return (a > 0 ? a : -a); }

	MATH_CALL(float) sqrt(float num);
	MATH_CALL(double) sqrt(double num);

	MATH_CALL(float) inversesqrt(float num);
	MATH_CALL(double) inversesqrt(double num);

	MATH_CALL(void) sin_cos(float _in, float* pSin, float* pCos);

	MATH_CALL(float) m_sin(float f);
	MATH_CALL(float) m_cos(float f);
	MATH_CALL(float) m_tan(float f);
	MATH_CALL(float) m_asin(float f);
	MATH_CALL(float) m_acos(float f);
	MATH_CALL(float) m_atan(float f);

	const float g_PI = 3.141592654f;
}
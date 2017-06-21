#include "MathLib.h"
#ifdef _WIN32
#include <Windows.h>
#endif

namespace Skuld
{
	const int32_t __fsqrt_guess_value = 0x5f375a86;
	const int64_t __dsqrt_guess_value = 0x5fe6ec85e7de30da;

	MATH_CALL(float) sqrt(float num)
	{
		float xhalf = 0.5f * num;
		int32_t i = __fsqrt_guess_value - (*reinterpret_cast<int32_t*>(&num) >> 1);
		num = *reinterpret_cast<float*>(&i);
		num = num*(1.5f - xhalf * num * num);
		num = num*(1.5f - xhalf * num * num);
		num = num*(1.5f - xhalf * num * num);
		return 1.0f / num;
	}

	MATH_CALL(double) sqrt(double num)
	{
		double xhalf = 0.5 * num;
		int64_t i = __dsqrt_guess_value - (*reinterpret_cast<int64_t*>(&num) >> 1);
		num = *reinterpret_cast<double*>(&i);
		num = num*(1.5 - xhalf * num * num);
		num = num*(1.5 - xhalf * num * num);
		num = num*(1.5 - xhalf * num * num);
		num = num*(1.5 - xhalf * num * num);
		return 1.0 / num;
	}

	MATH_CALL(double) inversesqrt(double num)
	{
		double xhalf = 0.5 * num;
		int64_t i = __dsqrt_guess_value - (*reinterpret_cast<int64_t*>(&num) >> 1);
		num = *reinterpret_cast<double*>(&i);
		num = num*(1.5 - xhalf * num * num);
		num = num*(1.5 - xhalf * num * num);
		num = num*(1.5 - xhalf * num * num);
		num = num*(1.5 - xhalf * num * num);
		return num;
	}

	static const float g_2PI = 6.283185307f;
	static const float g_1DIV2PI = 0.159154943f;
	static const float g_PIDIV2 = 1.570796327f;

	MATH_CALL(void) sin_cos(float _in, float* pSin, float* pCos)
	{
		if (pSin == nullptr || pCos == nullptr) return;
		float quotient = g_1DIV2PI*_in;
		if (_in >= 0.0f)
		{
			quotient = (float)((int)(quotient + 0.5f));
		}
		else
		{
			quotient = (float)((int)(quotient - 0.5f));
		}
		float y = _in - g_2PI*quotient;
		float sign;
		if (y > g_PIDIV2)
		{
			y = g_PI - y;
			sign = -1.0f;
		}
		else if (y < -g_PIDIV2)
		{
			y = -g_PI - y;
			sign = -1.0f;
		}
		else
		{
			sign = +1.0f;
		}
		float y2 = y * y;
		*pSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;
		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		*pCos = sign*p;
	}

	MATH_CALL(float) m_sin(float n)
	{
		float quotient = g_1DIV2PI*n;
		if (n >= 0.0f)
		{
			quotient = (float)((int)(quotient + 0.5f));
		}
		else
		{
			quotient = (float)((int)(quotient - 0.5f));
		}
		float y = n - g_2PI*quotient;
		if (y > g_PIDIV2)
		{
			y = g_PI - y;
		}
		else if (y < -g_PIDIV2)
		{
			y = -g_PI - y;
		}
		float y2 = y * y;
		return (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;
	}

	MATH_CALL(float) m_cos(float _in)
	{
		float quotient = g_1DIV2PI*_in;
		if (_in >= 0.0f)
		{
			quotient = (float)((int)(quotient + 0.5f));
		}
		else
		{
			quotient = (float)((int)(quotient - 0.5f));
		}
		float y = _in - g_2PI*quotient;
		float sign;
		if (y > g_PIDIV2)
		{
			y = g_PI - y;
			sign = -1.0f;
		}
		else if (y < -g_PIDIV2)
		{
			y = -g_PI - y;
			sign = -1.0f;
		}
		else
		{
			sign = +1.0f;
		}
		float y2 = y * y;
		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		return sign*p;
	}

	MATH_CALL(float) inversesqrt(float num)
	{
		float xhalf = 0.5f * num;
		int32_t i = __fsqrt_guess_value - (*reinterpret_cast<int32_t*>(&num) >> 1);
		num = *reinterpret_cast<float*>(&i);
		num = num*(1.5f - xhalf * num * num);
		num = num*(1.5f - xhalf * num * num);
		num = num*(1.5f - xhalf * num * num);
		return num;
	}
}
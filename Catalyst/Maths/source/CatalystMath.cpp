#include "camathspch.h"
#include "CatalystMath.h"

#include <cfloat>
#include <corecrt_math.h>

namespace Catalyst
{
	float CatalystMath::pi = 3.1415926535f;
	float CatalystMath::threshold = 0.000001f;

	bool CatalystMath::Compare(const float _x, const float _y)
	{
		return fabsf(_x - _y) <= (FLT_EPSILON + threshold) * fmaxf(1.f, fmaxf(fabsf(_x), fabsf(_y)));
	}

	float CatalystMath::Clamp(float _val, const float _min, const float _max)
	{
		if (_val < _min)
			_val = _min;

		if (_val > _max)
			_val = _max;

		return _val;
	}

	float CatalystMath::Clamp01(const float _val)
	{
		return Clamp(_val, 0.f, 1.f);
	}

	float CatalystMath::Radians(const float _deg)
	{
		return _deg * pi / 180.f;
	}

	float CatalystMath::Degrees(const float _rad)
	{
		return _rad * 180.f / pi;
	}
}

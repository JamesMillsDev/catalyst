#include "camathspch.h"
#include "Quaternion.h"

#include "CatalystMath.h"

namespace Catalyst
{
	Quaternion::Quaternion()
		: x{ 0 }, y{ 0 }, z{ 0 }, w{ 1 }
	{

	}

	Quaternion::Quaternion(const float _x, const float _y, const float _z, const float _w)
		: x{ _x }, y{ _y }, z{ _z }, w{ _w }
	{
	}

	Quaternion::Quaternion(const Vector3& _euler)
		: x{ 0 }, y{ 0 }, z{ 0 }, w{ 1 }
	{
		EulerAngles(_euler);
	}

	Quaternion::Quaternion(const float _pitch, const float _yaw, const float _roll)
		: x{ 0 }, y{ 0 }, z{ 0 }, w{ 1 }
	{
		EulerAngles(_pitch, _yaw, _roll);
	}

	Quaternion::~Quaternion() = default;

	Quaternion::Quaternion(const Quaternion& _other)
		: x{ _other.x }, y{ _other.y }, z{ _other.z }, w{ _other.w }
	{
	}

	Quaternion::Quaternion(Quaternion&& _other) noexcept
		: x{ _other.x }, y{ _other.y }, z{ _other.z }, w{ _other.w }
	{
		_other.x = 0.f;
		_other.y = 0.f;
		_other.z = 0.f;
		_other.w = 0.f;
	}

	Quaternion Quaternion::FromEuler(const Vector3& _euler)
	{
		return FromEuler(_euler.x, _euler.y, _euler.z);
	}

	Quaternion Quaternion::FromEuler(const float _pitch, const float _yaw, const float _roll)
	{
		Quaternion temp = {};

		temp.EulerAngles(_pitch, _yaw, _roll);

		return temp;
	}

	Vector3 Quaternion::Euler() const
	{
		Vector3 euler = { 0 };

		euler.x = asinf(2.f * x * y + 2.f * z * w);

		// North pole calculation
		if (CatalystMath::Compare(x * y + z * w, .5f))
		{
			euler.y = 2.f * atan2f(x, w);
			euler.z = 0.f;
		}
		// South pole calculation
		else if (CatalystMath::Compare(x * y + z * w, -.5f))
		{
			euler.y = -2.f * atan2f(x, w);
			euler.z = 0.f;
		}
		else
		{
			euler.y = atan2f(2.f * y * w - 2.f * x * z, 1.f - 2.f * powf(y, 2) - 2 * powf(z, 2));
			euler.z = atan2f(2.f * x * w - 2.f * y * z, 1.f - 2.f * powf(x, 2) - 2 * powf(z, 2));
		}

		return euler;
	}

	void Quaternion::EulerAngles(const Vector3& _euler)
	{
		EulerAngles(_euler.x, _euler.y, _euler.z);
	}

	void Quaternion::EulerAngles(const float _pitch, const float _yaw, const float _roll)
	{
		// Assuming the angles are in radians.
		const float c1 = cosf(_yaw / 2);
		const float s1 = sinf(_yaw / 2);
		const float c2 = cosf(_pitch / 2);
		const float s2 = sinf(_pitch / 2);
		const float c3 = cosf(_roll / 2);
		const float s3 = sinf(_roll / 2);

		const float c1c2 = c1 * c2;
		const float s1s2 = s1 * s2;

		w = c1c2 * c3 - s1s2 * s3;
		x = c1c2 * s3 + s1s2 * c3;
		y = s1 * c2 * c3 + c1 * s2 * s3;
		z = c1 * s2 * c3 - s1 * c2 * s3;
	}

	bool Quaternion::operator==(const Quaternion& _rhs) const
	{
		return CatalystMath::Compare(x, _rhs.x) &&
			CatalystMath::Compare(y, _rhs.y) &&
			CatalystMath::Compare(z, _rhs.z) &&
			CatalystMath::Compare(w, _rhs.w);
	}

	bool Quaternion::operator!=(const Quaternion& _rhs) const
	{
		return !(*this == _rhs);
	}

	Quaternion& Quaternion::operator=(const Quaternion& _other)
	{
		if (*this == _other)
			return *this;

		x = _other.x;
		y = _other.y;
		z = _other.z;
		w = _other.w;

		return *this;
	}
}

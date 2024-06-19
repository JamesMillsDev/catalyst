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

	Quaternion::Quaternion(Matrix3& _matrix)
	{
		w = sqrtf(1 + _matrix[0][0] + _matrix[1][1] + _matrix[2][2]) / 2.f;

		const float w4 = 4.f * w;

		x = (_matrix[2][1] - _matrix[1][2]) / w4;
		y = (_matrix[0][2] - _matrix[2][0]) / w4;
		z = (_matrix[1][0] - _matrix[0][1]) / w4;
	}

	Quaternion::Quaternion(Matrix4& _matrix)
	{
		w = sqrtf(1 + _matrix[0][0] + _matrix[1][1] + _matrix[2][2]) / 2.f;

		const float w4 = 4.f * w;

		x = (_matrix[2][1] - _matrix[1][2]) / w4;
		y = (_matrix[0][2] - _matrix[2][0]) / w4;
		z = (_matrix[1][0] - _matrix[0][1]) / w4;
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

	Vector3 Quaternion::Euler() const
	{
		Vector3 euler = { 0 };

		euler.x = asinf(2.f * x * y + 2.f * z * w);

		// North Pole calculation
		if (CatalystMath::Compare(x * y + z * w, .5f))
		{
			euler.y = 2.f * atan2f(x, w);
			euler.z = 0.f;
		}
		// South Pole calculation
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

		euler.x = CatalystMath::Degrees(euler.x);
		euler.y = CatalystMath::Degrees(euler.y);
		euler.z = CatalystMath::Degrees(euler.z);

		return euler;
	}

	void Quaternion::EulerAngles(const Vector3& _euler)
	{
		EulerAngles(_euler.x, _euler.y, _euler.z);
	}

	void Quaternion::EulerAngles(float _pitch, float _yaw, float _roll)
	{
		_pitch = CatalystMath::Radians(_pitch);
		_yaw = CatalystMath::Radians(_yaw);
		_roll = CatalystMath::Radians(_roll);

		// Assuming the angles are in radians.
		const float cosYaw = cosf(_yaw / 2);
		const float sinYaw = sinf(_yaw / 2);
		const float cosPitch = cosf(_pitch / 2);
		const float sinPitch = sinf(_pitch / 2);
		const float cosRoll = cosf(_roll / 2);
		const float sinRoll = sinf(_roll / 2);

		const float cosYawPitch = cosYaw * cosPitch;
		const float sinYawPitch = sinYaw * sinPitch;

		w = cosYawPitch * cosRoll - sinYawPitch * sinRoll;
		x = cosYawPitch * sinRoll + sinYawPitch * cosRoll;
		y = sinYaw * cosPitch * cosRoll + cosYaw * sinPitch * sinRoll;
		z = cosYaw * sinPitch * cosRoll - sinYaw * cosPitch * sinRoll;
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

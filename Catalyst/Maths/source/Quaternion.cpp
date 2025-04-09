#include "camathspch.h"
#include "Quaternion.h"

#include "CatalystMath.h"
#include "Matrix3.h"
#include "Matrix4.h"

namespace Catalyst
{
	Quaternion Quaternion::Identity()
	{
		return { 0.f, 0.f, 0.f, 1.f };
	}

	float Quaternion::Dot(const Quaternion& _a, const Quaternion& _b)
	{
		return _a.x * _b.x + _a.y * _b.y + _a.z * _b.z + _a.w * _b.w;
	}

	Quaternion Quaternion::LookRotation(const Vector3& _viewDir, const Vector3& _forward, const Vector3& _up)
	{
		Vector3 forward = _viewDir.Normalised();

		Vector3 rotAxis = Vector3::Cross(_forward, forward).Normalised();
		if (CatalystMath::Compare(rotAxis.MagnitudeSqr(), 0.f))
			rotAxis = _up;

		float dot = Vector3::Dot(Vector3::forward, _forward);
		float angle = acosf(dot);

		return AxisAngle(rotAxis, angle);
	}

	Quaternion Quaternion::AxisAngle(const Vector3& _axis, const float _angle)
	{
		const float halfAngle = _angle * .5f;
		const float sin = sinf(halfAngle);

		Vector3 axis = _axis.Normalised();

		return
		{
			axis.x * sin,
			axis.y * sin,
			axis.z * sin,
			cosf(halfAngle)
		};
	}

	Quaternion Quaternion::From(const float _pitch, const float _yaw, const float _roll)
	{
		return { _pitch, _yaw, _roll };
	}

	Quaternion Quaternion::From(const Vector3& _euler)
	{
		return { _euler };
	}

	Quaternion Quaternion::From(const Matrix3& _matrix)
	{
		return { _matrix };
	}

	Quaternion Quaternion::From(const Matrix4& _matrix)
	{
		return { _matrix };
	}

	Quaternion::Quaternion()
		: x{ 0.f }, y{ 0.f }, z{ 0.f }, w{ 0.f }
	{

	}

	Quaternion::Quaternion(const float _x, const float _y, const float _z, const float _w)
		: x{ _x }, y{ _y }, z{ _z }, w{ _w }
	{
	}

	Quaternion::Quaternion(const Vector3& _euler)
		: x{ 0 }, y{ 0 }, z{ 0 }, w{ 0.f }
	{
		EulerAngles(_euler);
	}

	Quaternion::Quaternion(const Matrix3& _matrix)
	{
		w = sqrtf(1 + _matrix[0][0] + _matrix[1][1] + _matrix[2][2]) / 2.f;

		const float w4 = 4.f * w;

		x = (_matrix[2][1] - _matrix[1][2]) / w4;
		y = (_matrix[0][2] - _matrix[2][0]) / w4;
		z = (_matrix[1][0] - _matrix[0][1]) / w4;
	}

	Quaternion::Quaternion(const Matrix4& _matrix)
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
		const float cr = cosf(_roll * 0.5f);
		const float sr = sinf(_roll * 0.5f);
		const float cp = cosf(_pitch * 0.5f);
		const float sp = sinf(_pitch * 0.5f);
		const float cy = cosf(_yaw * 0.5f);
		const float sy = sinf(_yaw * 0.5f);

		w = cr * cp * cy + sr * sp * sy;
		x = sr * cp * cy - cr * sp * sy;
		y = cr * sp * cy + sr * cp * sy;
		z = cr * cp * sy - sr * sp * cy;
	}

	string Quaternion::ToString() const
	{
		return string("(") + std::to_string(x) + string(", ") + std::to_string(y) + string(", ") + std::to_string(z) + string(", ") + std::to_string(w) + string(")");
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

	Quaternion Quaternion::operator*(const Quaternion& _rhs) const
	{
		return
		{
			w * _rhs.w - x * _rhs.x - y * _rhs.y - z * _rhs.z,
			w * _rhs.x + x * _rhs.w + y * _rhs.z - z * _rhs.y,
			w * _rhs.y - x * _rhs.z + y * _rhs.w + z * _rhs.x,
			w * _rhs.z + x * _rhs.y - y * _rhs.x + z * _rhs.w 
		};
	}

	Quaternion Quaternion::operator*(const Vector3& _rhs) const
	{
		float x = this->x * 2.f;
		float y = this->y * 2.f;
		float z = this->z * 2.f;

		float xx = this->x * x;
		float yy = this->y * y;
		float zz = this->z * z;

		float xy = this->x * y;
		float xz = this->x * z;
		float yz = this->y * z;

		float wx = this->w * x;
		float wy = this->w * y;
		float wz = this->w * z;

		return
		{
			(1.f - (yy + zz)) * _rhs.x + (xy - wz) * _rhs.y + (xz + wy) * _rhs.z,
			(xy + wz) * _rhs.x + (1.f - (xx + zz)) * _rhs.y + (yz - wx) * _rhs.z,
			(xz - wy) * _rhs.x + (yz + wx) * _rhs.y + (1.f - (xx + yy)) * _rhs.z
		};
	}

	float Quaternion::operator[](const int _index) const
	{
		if (_index < 0 || _index >= VEC_4_SIZE)
			return INFINITY;

		return data[_index];
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

	Quaternion& Quaternion::operator=(Quaternion&& _rhs) noexcept
	{
		if (*this == _rhs)
			return *this;

		x = _rhs.x;
		y = _rhs.y;
		z = _rhs.z;
		w = _rhs.w;

		_rhs.x = 0.f;
		_rhs.y = 0.f;
		_rhs.z = 0.f;
		_rhs.w = 0.f;

		return *this;
	}
}

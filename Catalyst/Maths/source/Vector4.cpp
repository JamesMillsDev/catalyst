#include "camathspch.h"
#include "Vector4.h"

#include "CatalystMath.h"

namespace Catalyst
{
	// Standard Vector4s
	Vector4 Vector4::zero = { 0.f, 0.f, 0.f, 0.f };
	Vector4 Vector4::one = { 1.f, 1.f, 1.f, 1.f };
	Vector4 Vector4::half = { .5f, .5f, .5f, .5f };
	Vector4 Vector4::up = { 0.f, 1.f, 0.f, 0.f };
	Vector4 Vector4::down = { 0.f, -1.f, 0.f, 0.f };
	Vector4 Vector4::left = { -1.f, 0.f, 0.f, 0.f };
	Vector4 Vector4::right = { 1.f, 0.f, 0.f, 0.f };
	Vector4 Vector4::forward = { 0.f, 0.f, 1.f, 0.f };
	Vector4 Vector4::back = { 0.f, 0.f, -1.f, 0.f };

	Vector4::Vector4()
		: x{ 0.f }, y{ 0.f }, z{ 0.f }, w{ 0.f }
	{

	}

	Vector4::Vector4(const float _scalar)
		: x{ _scalar }, y{ _scalar }, z{ _scalar }, w{ _scalar }
	{
	}

	Vector4::Vector4(const float _x, const float _y, const float _z, const float _w)
		:x{ _x }, y{ _y }, z{ _z }, w{ _w }
	{
	}

	Vector4::Vector4(const initializer_list<float> _values)
	{
		for (size_t i = 0; i < VEC_4_SIZE; ++i)
			data[i] = *(_values.begin() + i);
	}

	Vector4::~Vector4() = default;

	Vector4::Vector4(const Vector4& _other)
		:x{ _other.x }, y{ _other.y }, z{ _other.z }, w{ _other.w }
	{
	}

	Vector4::Vector4(Vector4&& _other) noexcept
		:x{ _other.x }, y{ _other.y }, z{ _other.z }, w{ _other.w }
	{
		_other.x = 0.f;
		_other.y = 0.f;
		_other.z = 0.f;
		_other.w = 0.f;
	}

	Vector4 Vector4::Normalised(const Vector4& _vec)
	{
		const float mag = _vec.Magnitude();

		if (CatalystMath::Compare(mag, 0.f))
		{
			return { 0.f, 0.f, 0.f, 0.f };
		}
		else
		{
			return { _vec.x / mag, _vec.y / mag , _vec.z / mag, _vec.w / mag };
		}
	}

	float Vector4::Distance(const Vector4& _a, const Vector4& _b)
	{
		return (_a - _b).Magnitude();
	}

	float Vector4::Dot(const Vector4& _lhs, const Vector4& _rhs)
	{
		return _lhs.x * _rhs.x + _lhs.y * _rhs.y + _lhs.z * _rhs.z + _lhs.w * _rhs.w;
	}

	Vector4 Vector4::Cross(const Vector4& _lhs, const Vector4& _rhs)
	{
		return
		{
			_lhs.y * _rhs.z - _lhs.z * _rhs.y,
			_lhs.z * _rhs.x - _lhs.x * _rhs.z,
			_lhs.x * _rhs.y - _lhs.y * _rhs.x,
			0.f
		};
	}

	Vector4 Vector4::Lerp(const Vector4& _a, const Vector4& _b, float _t)
	{
		_t = CatalystMath::Clamp01(_t);

		return _a * (1 - _t) + _b * _t;
	}

	float Vector4::Magnitude() const
	{
		return sqrtf(x * x + y * y + z * z + w * w);
	}

	float Vector4::MagnitudeSqr() const
	{
		return x * x + y * y + z * z + w * w;
	}

	void Vector4::Normalise()
	{
		const float mag = Magnitude();

		if (CatalystMath::Compare(mag, 0.f))
		{
			x = 0.f;
			y = 0.f;
			z = 0.f;
			w = 0.f;
		}
		else
		{
			x = x / mag;
			y = y / mag;
			z = z / mag;
			w = w / mag;
		}
	}

	Vector4 Vector4::Normalised() const
	{
		const float mag = Magnitude();

		if (CatalystMath::Compare(mag, 0.f))
		{
			return { 0.f, 0.f, 0.f, 0.f };
		}
		else
		{
			return { x / mag, y / mag , z / mag, w / mag };
		}
	}

	string Vector4::ToString() const
	{
		return string("(") + std::to_string(x) + string(", ") + std::to_string(y) + string(", ") + std::to_string(z) + string(", ") + std::to_string(w) + string(")");
	}

	bool Vector4::operator==(const Vector4& _rhs) const
	{
		return CatalystMath::Compare(x, _rhs.x) &&
			CatalystMath::Compare(y, _rhs.y) &&
			CatalystMath::Compare(z, _rhs.z) &&
			CatalystMath::Compare(w, _rhs.w);
	}

	bool Vector4::operator!=(const Vector4& _rhs) const
	{
		return !(*this == _rhs);
	}

	Vector4 Vector4::operator-(const Vector4& _rhs) const
	{
		return { x - _rhs.x, y - _rhs.y, z - _rhs.z,w - _rhs.w };
	}

	Vector4& Vector4::operator-=(const Vector4& _rhs)
	{
		x -= _rhs.x;
		y -= _rhs.y;
		z -= _rhs.z;
		w -= _rhs.w;

		return *this;
	}

	Vector4 Vector4::operator+(const Vector4& _rhs) const
	{
		return { x + _rhs.x, y + _rhs.y, z + _rhs.z, w + _rhs.w };
	}

	Vector4& Vector4::operator+=(const Vector4& _rhs)
	{
		x += _rhs.x;
		y += _rhs.y;
		z += _rhs.z;
		w += _rhs.w;

		return *this;
	}

	Vector4 Vector4::operator*(const Vector4& _rhs) const
	{
		return { x * _rhs.x, y * _rhs.y , z * _rhs.z, w * _rhs.w };
	}

	Vector4& Vector4::operator*=(const Vector4& _rhs)
	{
		x *= _rhs.x;
		y *= _rhs.y;
		z *= _rhs.z;
		w *= _rhs.w;
		return *this;
	}

	Vector4 Vector4::operator*(const float _rhs) const
	{
		return { x * _rhs, y * _rhs, z * _rhs, w * _rhs };
	}

	Vector4& Vector4::operator*=(const float _rhs)
	{
		x *= _rhs;
		y *= _rhs;
		z *= _rhs;
		w *= _rhs;
		return *this;
	}

	float Vector4::operator[](const int _index) const
	{
		if (_index < 0 || _index >= VEC_4_SIZE)
			return INFINITY;

		return data[_index];
	}

	Vector4& Vector4::operator=(const Vector4& _other)
	{
		if (*this == _other)
			return *this;

		x = _other.x;
		y = _other.y;
		z = _other.z;
		w = _other.w;

		return *this;
	}

	Vector4& Vector4::operator=(Vector4&& _rhs) noexcept
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

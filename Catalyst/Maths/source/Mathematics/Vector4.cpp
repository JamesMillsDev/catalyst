#include "camathspch.h"
#include "Mathematics/Vector4.h"

#include "Mathematics/Vector2.h"
#include "Mathematics/Vector3.h"

namespace Catalyst
{
	Vector4 Vector4::zero = { 0.f, 0.f, 0.f, 0.f };
	Vector4 Vector4::one = { 1.f, 1.f, 1.f, 1.f };
	Vector4 Vector4::half = { .5f, .5f, .5f, .5f };
	Vector4 Vector4::up = { 0.f, 0.f, 1.f, 0.f };
	Vector4 Vector4::down = { 0.f, 0.f, -1.f, 0.f };
	Vector4 Vector4::left = { 0.f, -1.f, 0.f, 0.f };
	Vector4 Vector4::right = { 0.f, 1.f, 0.f, 0.f };
	Vector4 Vector4::forward = { 1.f, 0.f, 0.f, 0.f };
	Vector4 Vector4::back = { -1.f, 0.f, 0.f, 0.f };

	Vector4::Vector4()
		: inner{ 0.f, 0.f, 0.f, 0.f }
	{
	}

	Vector4::Vector4(const float _scalar)
		: inner{ _scalar, _scalar, _scalar, _scalar }
	{
	}

	Vector4::Vector4(const float _x, const float _y, const float _z, const float _w)
		: inner{ _x, _y, _z, _w }
	{
	}

	Vector4::Vector4(const Vector3& _vec, const float _w)
		: inner{ static_cast<vec3>(_vec), _w }
	{
	}

	Vector4::Vector4(const vec4& _other)
		: inner{ _other }
	{
	}

	Vector4::Vector4(const Vector4& _other) = default;

	Vector4::Vector4(Vector4&& _other) noexcept
		: inner{ _other.inner }
	{
		_other.inner = {};
	}

	Vector4::~Vector4() = default;

	Vector4::operator glm::vec<4, float>() const
	{
		return inner;
	}

	bool Vector4::operator==(const Vector4& _rhs) const
	{
		return inner == _rhs.inner;
	}

	bool Vector4::operator!=(const Vector4& _rhs) const
	{
		return !(*this == _rhs);
	}

	Vector4 Vector4::operator+(const Vector4& _rhs) const
	{
		return inner + _rhs.inner;
	}

	Vector4& Vector4::operator+=(const Vector4& _rhs)
	{
		inner.x += _rhs.inner.x;
		inner.y += _rhs.inner.y;
		inner.z += _rhs.inner.z;
		inner.w += _rhs.inner.w;

		return *this;
	}

	Vector4 Vector4::operator-(const Vector4& _rhs) const
	{
		return inner - _rhs.inner;
	}

	Vector4& Vector4::operator-=(const Vector4& _rhs)
	{
		inner.x -= _rhs.inner.x;
		inner.y -= _rhs.inner.y;
		inner.z -= _rhs.inner.z;
		inner.w -= _rhs.inner.w;

		return *this;
	}

	Vector4 Vector4::operator*(const float _rhs) const
	{
		return inner * _rhs;
	}

	Vector4& Vector4::operator*=(const float _rhs)
	{
		inner.x *= _rhs;
		inner.y *= _rhs;
		inner.z *= _rhs;
		inner.w *= _rhs;

		return *this;
	}

	Vector4& Vector4::operator=(const Vector4& _other) = default;

	Vector4& Vector4::operator=(Vector4&& _other) noexcept
	{
		if (*this == _other)
			return *this;

		inner = _other.inner;
		_other.inner = {};

		return *this;
	}
}

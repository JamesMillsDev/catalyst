#include "camathspch.h"
#include "Mathematics/Vector2.h"

#include "Mathematics/Vector3.h"
#include "Mathematics/Vector4.h"

namespace Catalyst
{
	Vector2::Vector2()
		: inner{ 0.f, 0.f }
	{
	}

	Vector2::Vector2(const float _scalar)
		: inner{ _scalar, _scalar }
	{
	}

	Vector2::Vector2(const float _x, const float _y)
		: inner{ _x, _y }
	{
	}

	Vector2::Vector2(const Vector3& _vec)
		: inner{ static_cast<vec3>(_vec) }
	{
	}

	Vector2::Vector2(const Vector4& _vec)
		: inner{ static_cast<vec4>(_vec) }
	{
	}

	Vector2::Vector2(const vec2& _other)
		: inner{ _other }
	{
	}

	Vector2::Vector2(const Vector2& _other) = default;

	Vector2::Vector2(Vector2&& _other) noexcept
		: inner{ _other.inner }
	{
		_other.inner = {};
	}

	Vector2::~Vector2() = default;

	Vector2::operator glm::vec<2, float>() const
	{
		return inner;
	}

	bool Vector2::operator==(const Vector2& _rhs) const
	{
		return inner == _rhs.inner;
	}

	bool Vector2::operator!=(const Vector2& _rhs) const
	{
		return !(*this == _rhs);
	}

	Vector2 Vector2::operator+(const Vector2& _rhs) const
	{
		return inner + _rhs.inner;
	}

	Vector2& Vector2::operator+=(const Vector2& _rhs)
	{
		inner.x += _rhs.inner.x;
		inner.y += _rhs.inner.y;

		return *this;
	}

	Vector2 Vector2::operator-(const Vector2& _rhs) const
	{
		return inner - _rhs.inner;
	}

	Vector2& Vector2::operator-=(const Vector2& _rhs)
	{
		inner.x -= _rhs.inner.x;
		inner.y -= _rhs.inner.y;

		return *this;
	}

	Vector2 Vector2::operator*(const float _rhs) const
	{
		return inner * _rhs;
	}

	Vector2& Vector2::operator*=(const float _rhs)
	{
		inner.x *= _rhs;
		inner.y *= _rhs;

		return *this;
	}

	Vector2& Vector2::operator=(const Vector2& _other) = default;

	Vector2& Vector2::operator=(Vector2&& _other) noexcept
	{
		if (*this == _other)
			return *this;

		inner = _other.inner;
		_other.inner = {};

		return *this;
	}
}

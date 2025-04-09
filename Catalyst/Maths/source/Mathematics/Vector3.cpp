#include "camathspch.h"
#include "Mathematics/Vector3.h"

#include "Mathematics/Vector2.h"
#include "Mathematics/Vector4.h"

namespace Catalyst
{
	Vector3 Vector3::zero = { 0.f, 0.f, 0.f };
	Vector3 Vector3::one = { 1.f, 1.f, 1.f };
	Vector3 Vector3::half = { .5f, .5f, .5f };
	Vector3 Vector3::up = { 0.f, 0.f, 1.f };
	Vector3 Vector3::down = { 0.f, 0.f, -1.f };
	Vector3 Vector3::left = { 0.f, -1.f, 0.f };
	Vector3 Vector3::right = { 0.f, 1.f, 0.f };
	Vector3 Vector3::forward = { 1.f, 0.f, 0.f };
	Vector3 Vector3::back = { -1.f, 0.f, 0.f };

	Vector3::Vector3()
		: inner{ 0.f, 0.f, 0.f }
	{
	}

	Vector3::Vector3(const float _scalar)
		: inner{ _scalar, _scalar, _scalar }
	{
	}

	Vector3::Vector3(const float _x, const float _y, const float _z)
		: inner{ _x, _y, _z }
	{
	}

	Vector3::Vector3(const Vector4& _vec)
		: inner{ static_cast<vec4>(_vec) }
	{
	}

	Vector3::Vector3(const vec3& _other)
		: inner{ _other }
	{
	}

	Vector3::Vector3(const Vector3& _other) = default;

	Vector3::Vector3(Vector3&& _other) noexcept
		: inner{ _other.inner }
	{
		_other.inner = {};
	}

	Vector3::~Vector3() = default;

	Vector3::operator glm::vec<3, float>() const
	{
		return inner;
	}

	bool Vector3::operator==(const Vector3& _rhs) const
	{
		return inner == _rhs.inner;
	}

	bool Vector3::operator!=(const Vector3& _rhs) const
	{
		return !(*this == _rhs);
	}

	Vector3 Vector3::operator+(const Vector3& _rhs) const
	{
		return inner + _rhs.inner;
	}

	Vector3& Vector3::operator+=(const Vector3& _rhs)
	{
		inner.x += _rhs.inner.x;
		inner.y += _rhs.inner.y;
		inner.z += _rhs.inner.z;

		return *this;
	}

	Vector3 Vector3::operator-(const Vector3& _rhs) const
	{
		return inner - _rhs.inner;
	}

	Vector3& Vector3::operator-=(const Vector3& _rhs)
	{
		inner.x -= _rhs.inner.x;
		inner.y -= _rhs.inner.y;
		inner.z -= _rhs.inner.z;

		return *this;
	}

	Vector3 Vector3::operator*(const float _rhs) const
	{
		return inner * _rhs;
	}

	Vector3& Vector3::operator*=(const float _rhs)
	{
		inner.x *= _rhs;
		inner.y *= _rhs;
		inner.z *= _rhs;

		return *this;
	}

	Vector3& Vector3::operator=(const Vector3& _other) = default;

	Vector3& Vector3::operator=(Vector3&& _other) noexcept
	{
		if (*this == _other)
			return *this;

		inner = _other.inner;
		_other.inner = {};

		return *this;
	}
}

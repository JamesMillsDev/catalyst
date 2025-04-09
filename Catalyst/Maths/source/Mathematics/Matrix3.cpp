#include "camathspch.h"
#include "Mathematics/Matrix3.h"

namespace Catalyst
{
	Matrix3::Matrix3()
		: m_inner{
			0.f, 0.f, 0.f,
			0.f, 0.f, 0.f,
			0.f, 0.f, 0.f
		}
	{
	}

	Matrix3::Matrix3(const float _scalar)
		: m_inner{
			_scalar, 0.f, 0.f,
			0.f, _scalar, 0.f,
			0.f, 0.f, _scalar
		}
	{
	}

	Matrix3::Matrix3(const float _11, const float _21, const float _31,
		const float _12, const float _22, const float _32,
		const float _13, const float _23, const float _33)
		: m_inner{
			_11, _21, _31,
			_12, _22, _32,
			_13, _23, _33
		}
	{
	}

	Matrix3::Matrix3(const vec3& _x, const vec3& _y, const vec3& _z)
		: m_inner{ _x, _y, _z }
	{
	}

	Matrix3::Matrix3(const mat3& _other)
		: m_inner{ _other }
	{
	}

	Matrix3::Matrix3(const Matrix3& _other) = default;

	Matrix3::Matrix3(Matrix3&& _other) noexcept
		: m_inner{ _other.m_inner }
	{
		_other.m_inner = {};
	}

	Matrix3::~Matrix3() = default;

	Matrix3::operator glm::mat<3, 3, float>() const
	{
		return m_inner;
	}

	bool Matrix3::operator==(const Matrix3& _rhs) const
	{
		return m_inner == _rhs.m_inner;
	}

	bool Matrix3::operator!=(const Matrix3& _rhs) const
	{
		return !(*this == _rhs);
	}

	Matrix3 Matrix3::operator*(const Matrix3& _rhs) const
	{
		return m_inner * _rhs.m_inner;
	}

	Matrix3& Matrix3::operator=(const Matrix3& _other) = default;

	Matrix3& Matrix3::operator=(Matrix3&& _other) noexcept
	{
		if (*this == _other)
			return *this;

		m_inner = _other.m_inner;
		_other.m_inner = {};

		return *this;
	}
}

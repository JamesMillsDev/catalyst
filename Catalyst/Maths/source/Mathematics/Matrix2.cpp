#include "camathspch.h"
#include "Mathematics/Matrix2.h"

namespace Catalyst
{
	Matrix2::Matrix2()
		: m_inner{
			0.f, 0.f,
			0.f, 0.f
		}
	{
	}

	Matrix2::Matrix2(const float _scalar)
		: m_inner{
			_scalar, 0.f,
			0.f, _scalar
		}
	{
	}

	Matrix2::Matrix2(const float _11, const float _21,
		const float _12, const float _22)
			: m_inner{
				_11, _21,
				_12, _22
			}
	{
	}

	Matrix2::Matrix2(const vec2& _x, const vec2& _y)
		: m_inner{ _x, _y }
	{
	}

	Matrix2::Matrix2(const mat2& _other)
		: m_inner{ _other }
	{
	}

	Matrix2::Matrix2(const Matrix2& _other) = default;

	Matrix2::Matrix2(Matrix2&& _other) noexcept
		: m_inner{ _other.m_inner }
	{
		_other.m_inner = {};
	}

	Matrix2::~Matrix2() = default;

	Matrix2::operator glm::mat<2, 2, float>() const
	{
		return m_inner;
	}

	bool Matrix2::operator==(const Matrix2& _rhs) const
	{
		return m_inner == _rhs.m_inner;
	}

	bool Matrix2::operator!=(const Matrix2& _rhs) const
	{
		return !(*this == _rhs);
	}

	Matrix2 Matrix2::operator*(const Matrix2& _rhs) const
	{
		return m_inner * _rhs.m_inner;
	}

	Matrix2& Matrix2::operator=(const Matrix2& _other) = default;

	Matrix2& Matrix2::operator=(Matrix2&& _other) noexcept
	{
		if (*this == _other)
			return *this;

		m_inner = _other.m_inner;
		_other.m_inner = {};

		return *this;
	}
}

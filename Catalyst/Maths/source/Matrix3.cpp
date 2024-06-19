#include "camathspch.h"
#include "Matrix3.h"

#include "Quaternion.h"

namespace Catalyst
{
	Matrix3::Matrix3()
		: xAxis{ Vector3::zero }, yAxis{ Vector3::zero }, zAxis{ Vector3::zero }
	{

	}

	Matrix3::Matrix3(const Quaternion& _quat)
	{
		const float sqrQx = _quat.x * _quat.x;
		const float sqrQy = _quat.y * _quat.y;
		const float sqrQz = _quat.z * _quat.z;

		xAxis =
		{
			1.f - 2.f * sqrQy - 2.f * sqrQz,
			2.f * _quat.x * _quat.y + 2.f * _quat.z * _quat.w,
			2.f * _quat.x * _quat.z - 2.f * _quat.y * _quat.w
		};

		yAxis =
		{
			2.f * _quat.x * _quat.y - 2.f * _quat.z * _quat.w,
			1.f - 2.f * sqrQx - 2.f * sqrQz,
			2.f * _quat.y * _quat.z + 2.f * _quat.x * _quat.w
		};

		zAxis =
		{
			2.f * _quat.x * _quat.z + 2.f * _quat.y * _quat.w,
			2.f * _quat.y * _quat.z - 2.f * _quat.x * _quat.w,
			1.f - 2.f * sqrQx - 2.f * sqrQy
		};
	}

	Matrix3::Matrix3(Vector3 _xAxis, Vector3 _yAxis, Vector3 _zAxis)
		: xAxis{ std::move(_xAxis) }, yAxis{ std::move(_yAxis) }, zAxis{ std::move(_zAxis) }
	{
	}

	Matrix3::Matrix3(const float _11, const float _12, const float _13,
		const float _21, const float _22, const float _23,
		const float _31, const float _32, const float _33)
	{
		data[0][0] = _11;
		data[0][1] = _12;
		data[0][2] = _13;

		data[1][0] = _21;
		data[1][1] = _22;
		data[1][2] = _23;

		data[2][0] = _31;
		data[2][1] = _32;
		data[2][2] = _33;
	}

	Matrix3::~Matrix3()
	{
	}

	Matrix3::Matrix3(const Matrix3& _other)
		: xAxis{ _other.xAxis }, yAxis{ _other.yAxis }, zAxis{ _other.zAxis }
	{
	}

	bool Matrix3::operator==(const Matrix3& _rhs) const
	{
		for (int i = 0; i < VEC_3_SIZE; ++i)
		{
			if ((*this)[i] != _rhs[i])
				return false;
		}

		return true;
	}

	bool Matrix3::operator!=(const Matrix3& _rhs) const
	{
		return !(*this == _rhs);
	}

	Matrix3 Matrix3::operator*(const Matrix3& _rhs) const
	{
		return
		{
			{
				data[0][0] * _rhs[0][0] + data[0][1] * _rhs[1][0] + data[0][2] * _rhs[2][0],
				data[0][0] * _rhs[0][1] + data[0][1] * _rhs[1][1] + data[0][2] * _rhs[2][1],
				data[0][0] * _rhs[0][2] + data[0][1] * _rhs[1][2] + data[0][2] * _rhs[2][2]
			},
			{
				data[1][0] * _rhs[0][0] + data[1][1] * _rhs[1][0] + data[1][2] * _rhs[2][0],
				data[1][0] * _rhs[0][1] + data[1][1] * _rhs[1][1] + data[1][2] * _rhs[2][1],
				data[1][0] * _rhs[0][2] + data[1][1] * _rhs[1][2] + data[1][2] * _rhs[2][2]
			},
			{
				data[2][0] * _rhs[0][0] + data[2][1] * _rhs[1][0] + data[2][2] * _rhs[2][0],
				data[2][0] * _rhs[0][1] + data[2][1] * _rhs[1][1] + data[2][2] * _rhs[2][1],
				data[2][0] * _rhs[0][2] + data[2][1] * _rhs[1][2] + data[2][2] * _rhs[2][2]
			}
		};
	}

	Vector3 Matrix3::operator*(const Vector3& _rhs) const
	{
		return
		{
			data[0][0] * _rhs[0] + data[0][1] * _rhs[1] + data[0][2] * _rhs[2],
			data[1][0] * _rhs[0] + data[1][1] * _rhs[1] + data[1][2] * _rhs[2],
			data[2][0] * _rhs[0] + data[2][1] * _rhs[1] + data[2][2] * _rhs[2]
		};
	}

	Vector3 Matrix3::operator[](const int _index) const
	{
		switch (_index)
		{
		case 0:
			return xAxis;
		case 1:
			return yAxis;
		case 2:
			return zAxis;
		default:
			return Vector3::zero;

		}
	}

	Matrix3& Matrix3::operator=(const Matrix3& _rhs)
	{
		if (*this == _rhs)
			return *this;

		xAxis = _rhs.xAxis;
		yAxis = _rhs.yAxis;
		zAxis = _rhs.zAxis;

		return *this;
	}

	Matrix3& Matrix3::operator=(Matrix3&& _rhs) noexcept
	{
		if (*this == _rhs)
			return *this;

		xAxis = _rhs.xAxis;
		yAxis = _rhs.yAxis;
		zAxis = _rhs.zAxis;

		_rhs.xAxis = {};
		_rhs.yAxis = {};
		_rhs.zAxis = {};

		return *this;
	}
}

#include "camathspch.h"
#include "Matrix4.h"

#include "Matrix3.h"
#include "Quaternion.h"

namespace Catalyst
{
	Matrix4::Matrix4()
		: xAxis{ 0.f, 0.f, 0.f, 0.f }, yAxis{ 0.f, 0.f, 0.f, 0.f },
		zAxis{ 0.f, 0.f, 0.f, 0.f }, wAxis{ 0.f, 0.f, 0.f, 0.f }
	{

	}

	Matrix4::Matrix4(const Matrix3& _rhs)
	{
		xAxis = { _rhs.xAxis.x, _rhs.xAxis.y, _rhs.xAxis.z, 0.f };
		yAxis = { _rhs.yAxis.x, _rhs.yAxis.y, _rhs.yAxis.z, 0.f };
		zAxis = { _rhs.zAxis.x, _rhs.zAxis.y, _rhs.zAxis.z, 0.f };
		wAxis = { 0.f, 0.f, 0.f, 1.f };
	}

	Matrix4::Matrix4(const Quaternion& _quat)
	{
		const float sqrQx = _quat.x * _quat.x;
		const float sqrQy = _quat.y * _quat.y;
		const float sqrQz = _quat.z * _quat.z;

		xAxis =
		{
			1.f - 2.f * sqrQy - 2.f * sqrQz,
			2.f * _quat.x * _quat.y + 2.f * _quat.z * _quat.w,
			2.f * _quat.x * _quat.z - 2.f * _quat.y * _quat.w,
			0.f
		};

		yAxis =
		{
			2.f * _quat.x * _quat.y - 2.f * _quat.z * _quat.w,
			1.f - 2.f * sqrQx - 2.f * sqrQz,
			2.f * _quat.y * _quat.z + 2.f * _quat.x * _quat.w,
			0.f
		};

		zAxis =
		{
			2.f * _quat.x * _quat.z + 2.f * _quat.y * _quat.w,
			2.f * _quat.y * _quat.z - 2.f * _quat.x * _quat.w,
			1.f - 2.f * sqrQx - 2.f * sqrQy,
			0.f
		};

		wAxis = { 0.f, 0.f, 0.f, 1.f };
	}

	Matrix4::Matrix4(Vector4 _xAxis, Vector4 _yAxis, Vector4 _zAxis, Vector4 _wAxis)
		: xAxis{ std::move(_xAxis) }, yAxis{ std::move(_yAxis) }, zAxis{ std::move(_zAxis) }, wAxis{ std::move(_wAxis) }
	{
	}

	Matrix4::Matrix4(const float _11, const float _12, const float _13, const float _14, 
		const float _21, const float _22, const float _23, const float _24,
		const float _31, const float _32, const float _33, const float _34,
		const float _41, const float _42, const float _43, const float _44)
	{
		data[0][0] = _11;
		data[0][1] = _12;
		data[0][2] = _13;
		data[0][3] = _14;

		data[1][0] = _21;
		data[1][1] = _22;
		data[1][2] = _23;
		data[1][3] = _24;

		data[2][0] = _31;
		data[2][1] = _32;
		data[2][2] = _33;
		data[2][3] = _34;

		data[3][0] = _41;
		data[3][1] = _42;
		data[3][2] = _43;
		data[3][3] = _44;
	}

	Matrix4::~Matrix4() = default;

	Matrix4::Matrix4(const Matrix4& _other)
		: xAxis{ _other.xAxis }, yAxis{ _other.yAxis }, zAxis{ _other.zAxis }, wAxis{ _other.wAxis }
	{
	}

	bool Matrix4::operator==(const Matrix4& _rhs) const
	{
		for (int i = 0; i < VEC_4_SIZE; ++i)
		{
			if ((*this)[i] != _rhs[i])
				return false;
		}

		return true;
	}

	bool Matrix4::operator!=(const Matrix4& _rhs) const
	{
		return !(*this == _rhs);
	}

	//todo: this
	Matrix4 Matrix4::operator*(const Matrix4& _rhs) const
	{
		return
		{
			{
				data[0][0] * _rhs[0][0] + data[0][1] * _rhs[1][0] + data[0][2] * _rhs[2][0] + data[0][2] * _rhs[2][0],
				data[0][0] * _rhs[0][1] + data[0][1] * _rhs[1][1] + data[0][2] * _rhs[2][1] + data[0][2] * _rhs[2][1],
				data[0][0] * _rhs[0][2] + data[0][1] * _rhs[1][2] + data[0][2] * _rhs[2][2] + data[0][2] * _rhs[2][2],
				data[0][0] * _rhs[0][2] + data[0][1] * _rhs[1][2] + data[0][2] * _rhs[2][2] + data[0][2] * _rhs[2][2]
			},
			{
				data[1][0] * _rhs[0][0] + data[1][1] * _rhs[1][0] + data[1][2] * _rhs[2][0] + data[0][2] * _rhs[2][0],
				data[1][0] * _rhs[0][1] + data[1][1] * _rhs[1][1] + data[1][2] * _rhs[2][1] + data[0][2] * _rhs[2][1],
				data[1][0] * _rhs[0][2] + data[1][1] * _rhs[1][2] + data[1][2] * _rhs[2][2] + data[0][2] * _rhs[2][2],
				data[1][0] * _rhs[0][2] + data[0][1] * _rhs[1][2] + data[0][2] * _rhs[2][2] + data[0][2] * _rhs[2][2]
			},
			{
				data[2][0] * _rhs[0][0] + data[2][1] * _rhs[1][0] + data[2][2] * _rhs[2][0] + data[0][2] * _rhs[2][0],
				data[2][0] * _rhs[0][1] + data[2][1] * _rhs[1][1] + data[2][2] * _rhs[2][1] + data[0][2] * _rhs[2][1],
				data[2][0] * _rhs[0][2] + data[2][1] * _rhs[1][2] + data[2][2] * _rhs[2][2] + data[0][2] * _rhs[2][2],
				data[2][0] * _rhs[0][3] + data[2][1] * _rhs[1][3] + data[2][2] * _rhs[2][3] + data[0][2] * _rhs[2][2]
			},
			{
				data[3][0] * _rhs[0][0] + data[3][1] * _rhs[1][0] + data[3][2] * _rhs[2][0] + data[0][2] * _rhs[2][0],
				data[3][0] * _rhs[0][1] + data[3][1] * _rhs[1][1] + data[3][2] * _rhs[2][1] + data[0][2] * _rhs[2][1],
				data[3][0] * _rhs[0][2] + data[3][1] * _rhs[1][2] + data[3][2] * _rhs[2][2] + data[0][2] * _rhs[2][2],
				data[3][0] * _rhs[0][3] + data[3][1] * _rhs[1][3] + data[3][2] * _rhs[2][3] + data[0][2] * _rhs[2][2]
			}
		};
	}

	Vector4 Matrix4::operator*(const Vector4& _rhs) const
	{
		return
		{
			data[0][0] * _rhs[0] + data[0][1] * _rhs[1] + data[0][2] * _rhs[2] + data[0][3] * _rhs[3],
			data[1][0] * _rhs[0] + data[1][1] * _rhs[1] + data[1][2] * _rhs[2] + data[1][3] * _rhs[3],
			data[2][0] * _rhs[0] + data[2][1] * _rhs[1] + data[2][2] * _rhs[2] + data[2][3] * _rhs[3],
			data[3][0] * _rhs[0] + data[3][1] * _rhs[1] + data[3][2] * _rhs[2] + data[3][3] * _rhs[3]
		};
	}

	Vector4 Matrix4::operator[](const int _index) const
	{
		switch (_index)
		{
		case 0:
			return xAxis;
		case 1:
			return yAxis;
		case 2:
			return zAxis;
		case 3:
			return wAxis;
		default:
			return Vector4::zero;

		}
	}

	Matrix4& Matrix4::operator=(const Matrix4& _rhs)
	{
		if (*this == _rhs)
			return *this;

		xAxis = _rhs.xAxis;
		yAxis = _rhs.yAxis;
		zAxis = _rhs.zAxis;
		wAxis = _rhs.wAxis;

		return *this;
	}

	Matrix4& Matrix4::operator=(Matrix4&& _rhs) noexcept
	{
		if (*this == _rhs)
			return *this;

		xAxis = _rhs.xAxis;
		yAxis = _rhs.yAxis;
		zAxis = _rhs.zAxis;
		wAxis = _rhs.wAxis;

		_rhs.xAxis = {};
		_rhs.yAxis = {};
		_rhs.zAxis = {};
		_rhs.wAxis = {};

		return *this;
	}
}

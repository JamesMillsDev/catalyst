#include "camathspch.h"
#include "Matrix4.h"

#include "Quaternion.h"

namespace Catalyst
{
	Matrix4::Matrix4()
		: xAxis{ 1.f, 0.f, 0.f, 0.f }, yAxis{ 0.f, 1.f, 0.f, 0.f },
		zAxis{ 0.f, 0.f, 1.f, 0.f }, wAxis{ 0.f, 0.f, 0.f, 1.f }
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

	Matrix4::~Matrix4() = default;

	Matrix4::Matrix4(const Matrix4& _other)
	{
	}

	Matrix4& Matrix4::operator=(const Matrix4& _other)
	{
		return *this;
	}

	Vector4& Matrix4::operator[](const int _index)
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
}

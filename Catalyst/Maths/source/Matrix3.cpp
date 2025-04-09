#include "camathspch.h"
#include "Matrix3.h"

#include "Quaternion.h"

#include <sstream>

#include "CatalystMath.h"

using std::stringstream;

namespace Catalyst
{
	Matrix3 Matrix3::Identity()
	{
		return
		{
			1.f, 0.f, 0.f,
			0.f, 1.f, 0.f,
			0.f, 0.f, 1.f
		};
	}

	Matrix3 Matrix3::MakePitch(float _pitch)
	{
		_pitch = CatalystMath::Radians(_pitch);

		return
		{
			1.f, 0.f, 0.f,
			0.f, cosf(_pitch), sinf(_pitch),
			0.f, -sinf(_pitch), cosf(_pitch)
		};
	}

	Matrix3 Matrix3::MakeYaw(float _yaw)
	{
		_yaw = CatalystMath::Radians(_yaw);

		return
		{
			cosf(_yaw), 0.f, -sinf(_yaw),
			0.f, 1.f, 0.f,
			sinf(_yaw), 0.f, cosf(_yaw)
		};
	}

	Matrix3 Matrix3::MakeRoll(float _roll)
	{
		_roll = CatalystMath::Radians(_roll);

		return
		{
			cosf(_roll), sinf(_roll), 0.f,
			-sinf(_roll), cosf(_roll), 0.f,
			0.f, 0.f, 1.f
		};
	}

	Matrix3 Matrix3::MakeRotation(const Quaternion& _rotation)
	{
		return { _rotation };
	}

	Matrix3 Matrix3::MakeRotation(const float _pitch, const float _yaw, const float _roll)
	{
		return MakeRotation(Quaternion::From(_pitch, _yaw, _roll));
	}

	Matrix3 Matrix3::MakeRotation(const Vector3& _euler)
	{
		return MakeRotation(Quaternion::From(_euler));
	}

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

	void Matrix3::SetPitch(const float _pitch)
	{
		// Get the magnitude of the y and z-axis
		const float yLen = yAxis.Magnitude();
		const float zLen = zAxis.Magnitude();

		// Assign the y and z-axis' y and z values to the new rotation
		yAxis.y = cosf(_pitch) * yLen;
		zAxis.y = sinf(_pitch) * zLen;
		yAxis.z = -sinf(_pitch) * yLen;
		zAxis.z = cosf(_pitch) * zLen;
	}

	float Matrix3::Pitch() const
	{
		return atan2f(xAxis.y, xAxis.x);
	}

	void Matrix3::SetYaw(const float _yaw)
	{
		// Get the magnitude of the x and z-axis
		const float xLen = xAxis.Magnitude();
		const float zLen = zAxis.Magnitude();

		// Assign the x and z-axis' x and z values to the new rotation
		xAxis.x = cosf(_yaw) * xLen;
		zAxis.x = -sinf(_yaw) * zLen;
		xAxis.z = sinf(_yaw) * xLen;
		zAxis.z = cosf(_yaw) * zLen;
	}

	float Matrix3::Yaw() const
	{
		return atan2f(-yAxis.x, yAxis.y);
	}

	void Matrix3::SetRoll(const float _roll)
	{
		// Get the magnitude of the x and y-axis
		const float xLen = xAxis.Magnitude();
		const float yLen = yAxis.Magnitude();

		// Assign the x and y-axis' x and y values to the new rotation
		xAxis.x = cosf(_roll) * xLen;
		yAxis.x = sinf(_roll) * yLen;
		xAxis.y = -sinf(_roll) * xLen;
		yAxis.y = cosf(_roll) * yLen;
	}

	float Matrix3::Roll() const
	{
		return atan2f(zAxis.x, zAxis.z);
	}

	void Matrix3::SetRotation(const float _pitch, const float _yaw, const float _roll)
	{
		SetPitch(_pitch);
		SetYaw(_yaw);
		SetRoll(_roll);
	}

	void Matrix3::SetRotation(const Vector3& _euler)
	{
		SetRotation(_euler.x, _euler.y, _euler.z);
	}

	void Matrix3::SetRotation(const Quaternion& _quat)
	{
		SetRotation(_quat.Euler());
	}

	Quaternion Matrix3::Rotation() const
	{
		return { *this };
	}

	Vector3 Matrix3::Euler() const
	{
		return Quaternion(*this).Euler();
	}

	void Matrix3::Rotation(float* _pitch, float* _yaw, float* _roll) const
	{
		const Vector3 rotation = Euler();

		// Assign the incoming pitch pointer to the pitch rotation provided it is not nullptr
		if (_pitch)
			*_pitch = rotation.x;

		// Assign the incoming yaw pointer to the yaw rotation provided it is not nullptr
		if (_yaw)
			*_yaw = rotation.y;

		// Assign the incoming roll pointer to the roll rotation provided it is not nullptr
		if (_roll)
			*_roll = rotation.z;
	}

	Matrix3 Matrix3::Transposed()
	{
		return
		{
			data[0][0], data[1][0], data[2][0],
			data[0][1], data[1][1], data[2][1],
			data[0][2], data[1][2], data[2][2]
		};
	}

	string Matrix3::ToString() const
	{
		// Begin the building of a string stream with an opening bracket
		stringstream stream;
		stream << "(";

		// Cycle through the rows and columns
		for (int r = 0; r < VEC_3_SIZE; ++r)
		{
			auto& row = data[r];

			for (int c = 0; c < VEC_3_SIZE; ++c)
			{
				// Push the value into the stream
				stream << row[c];

				// If the current column is not the final one, add a ','
				if (c + 1 < VEC_3_SIZE)
					stream << ", ";
			}

			// If the current row is not the final one, add a ','
			if (r + 1 < VEC_3_SIZE)
				stream << ", ";
		}

		// Finalise the stream using a closing bracket
		stream << ")";
		return stream.str();
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

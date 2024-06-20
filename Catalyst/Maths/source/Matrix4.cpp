#include "camathspch.h"
#include "Matrix4.h"

#include "Matrix3.h"
#include "Quaternion.h"

#include <sstream>

using std::stringstream;

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

	Matrix4 Matrix4::Identity()
	{
		return
		{
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		};
	}

	Matrix4 Matrix4::MakeTranslation(const float _x, const float _y, const float _z)
	{
		return
		{
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			_x, _y, _z, 1.f
		};
	}

	Matrix4 Matrix4::MakeTranslation(const Vector3& _translation)
	{
		return MakeTranslation(_translation.x, _translation.y, _translation.z);
	}

	Matrix4 Matrix4::MakePitch(const float _pitch)
	{
		return
		{
			1.f, 0.f, 0.f, 0.f,
			0.f, cosf(_pitch), sinf(_pitch), 0.f,
			0.f, -sinf(_pitch), cosf(_pitch), 0.f,
			0.f, 0.f, 0.f, 1.f
		};
	}

	Matrix4 Matrix4::MakeYaw(const float _yaw)
	{
		return
		{
			cosf(_yaw), 0.f, -sinf(_yaw), 0.f,
			0.f, 1.f, 0.f, 0.f,
			sinf(_yaw), 0.f, cosf(_yaw), 0.f,
			0.f, 0.f, 0.f, 1.f
		};
	}

	Matrix4 Matrix4::MakeRoll(const float _roll)
	{
		return
		{
			cosf(_roll), sinf(_roll), 0.f, 0.f,
			-sinf(_roll), cosf(_roll), 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		};
	}

	Matrix4 Matrix4::MakeRotation(const Quaternion& _rotation)
	{
		return { _rotation };
	}

	Matrix4 Matrix4::MakeRotation(const float _pitch, const float _yaw, const float _roll)
	{
		return MakeRotation(Vector3{ _pitch, _yaw, _roll });
	}

	Matrix4 Matrix4::MakeRotation(const Vector3& _euler)
	{
		return MakeRotation({ _euler });
	}

	Matrix4 Matrix4::MakeScale(float _x, float _y, float _z)
	{
		return
		{
			_x, 0.f, 0.f, 0.f,
			0.f, _y, 0.f, 0.f,
			0.f, 0.f, _z, 0.f,
			0.f, 0.f, 0.f, 1.f
		};
	}

	Matrix4 Matrix4::MakeScale(const Vector3& _scale)
	{
		return MakeScale(_scale.x, _scale.y, _scale.z);
	}

	void Matrix4::SetTranslation(const float _x, const float _y, const float _z)
	{
		// Override the scale with the incoming one.
		wAxis.x = _x;
		wAxis.y = _y;
		wAxis.z = _z;
	}

	void Matrix4::SetTranslation(const Vector3& _translation)
	{
		SetTranslation(_translation.x, _translation.y, _translation.z);
	}

	void Matrix4::Translate(const float _x, const float _y, const float _z)
	{
		// Add the current translation to the incoming one
		wAxis.x += _x;
		wAxis.y += _y;
		wAxis.z += _z;
	}

	void Matrix4::Translate(const Vector3& _translation)
	{
		Translate(_translation.x, _translation.y, _translation.z);
	}

	Vector3 Matrix4::Translation() const
	{
		return { wAxis.x, wAxis.y, wAxis.z };
	}

	void Matrix4::Translation(float* _x, float* _y, float* _z) const
	{
		// Assign the incoming x pointer to the x translation provided it is not nullptr
		if (_x)
			*_x = wAxis.x;

		// Assign the incoming y pointer to the y translation provided it is not nullptr
		if (_y)
			*_y = wAxis.y;

		// Assign the incoming z pointer to the z translation provided it is not nullptr
		if (_z)
			*_z = wAxis.z;
	}

	void Matrix4::SetPitch(const float _pitch)
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

	float Matrix4::Pitch() const
	{
		return atan2f(xAxis.y, xAxis.x);
	}

	void Matrix4::SetYaw(const float _yaw)
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

	float Matrix4::Yaw() const
	{
		return atan2f(-yAxis.x, yAxis.y);
	}

	void Matrix4::SetRoll(const float _roll)
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

	float Matrix4::Roll() const
	{
		return atan2f(zAxis.x, zAxis.z);
	}

	void Matrix4::SetRotation(const float _pitch, const float _yaw, const float _roll)
	{
		SetPitch(_pitch);
		SetYaw(_yaw);
		SetRoll(_roll);
	}

	void Matrix4::SetRotation(const Vector3& _euler)
	{
		SetRotation(_euler.x, _euler.y, _euler.z);
	}

	void Matrix4::SetRotation(const Quaternion& _quat)
	{
		SetRotation(_quat.Euler());
	}

	Quaternion Matrix4::Rotation() const
	{
		return { *this };
	}

	Vector3 Matrix4::Euler() const
	{
		return Quaternion(*this).Euler();
	}

	void Matrix4::Rotation(float* _pitch, float* _yaw, float* _roll) const
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

	void Matrix4::SetScale(const float _x, const float _y, const float _z)
	{
		// Get all 3 axis magnitudes.
		const float xLen = xAxis.Magnitude();
		const float yLen = yAxis.Magnitude();
		const float zLen = zAxis.Magnitude();

		// Prevent negative and nan scaling on the x-axis
		if(xLen > 0.f)
		{
			xAxis.x /= xLen * _x;
			xAxis.y /= xLen * _x;
			xAxis.z /= xLen * _x;
		}

		// Prevent negative and nan scaling on the y-axis
		if (yLen > 0.f)
		{
			yAxis.x /= yLen * _y;
			yAxis.y /= yLen * _y;
			yAxis.z /= yLen * _y;
		}

		// Prevent negative and nan scaling on the z-axis
		if (zLen > 0.f)
		{
			zAxis.x /= zLen * _z;
			zAxis.y /= zLen * _z;
			zAxis.z /= zLen * _z;
		}
	}

	void Matrix4::SetScale(const Vector3& _scale)
	{
		SetScale(_scale.x, _scale.y, _scale.z);
	}

	Vector3 Matrix4::Scale() const
	{
		return
		{
			xAxis.Magnitude(),
			yAxis.Magnitude(),
			zAxis.Magnitude()
		};
	}

	void Matrix4::Scale(float* _x, float* _y, float* _z) const
	{
		const Vector3 scale = Scale();

		// Assign the incoming x pointer to the x scale provided it is not nullptr
		if (_x)
			*_x = scale.x;

		// Assign the incoming y pointer to the y scale provided it is not nullptr
		if (_y)
			*_y = scale.y;

		// Assign the incoming z pointer to the z scale provided it is not nullptr
		if (_z)
			*_z = scale.z;
	}

	Matrix4 Matrix4::Transposed()
	{
		return
		{
			data[0][0], data[1][0], data[2][0], data[3][0],
			data[0][1], data[1][1], data[2][1], data[3][1],
			data[0][2], data[1][2], data[2][2], data[3][2],
			data[0][3], data[1][3], data[2][3], data[3][3]
		};
	}

	string Matrix4::ToString() const
	{
		// Begin the building of a string stream with an opening bracket
		stringstream stream;
		stream << "(";

		// Cycle through the rows and columns
		for (int r = 0; r < VEC_4_SIZE; ++r)
		{
			auto& row = data[r];

			for (int c = 0; c < VEC_4_SIZE; ++c)
			{
				// Push the value into the stream
				stream << row[c];

				// If the current column is not the final one, add a ','
				if (c + 1 < VEC_4_SIZE)
					stream << ", ";
			}

			// If the current row is not the final one, add a ','
			if (r + 1 < VEC_4_SIZE)
				stream << ", ";
		}

		// Finalise the stream using a closing bracket
		stream << ")";
		return stream.str();
	}

	bool Matrix4::operator==(const Matrix4& _rhs) const
	{
		for (int i = 0; i < VEC_4_SIZE; ++i)
		{
			// Compare each row to the corresponding rhs row
			if ((*this)[i] != _rhs[i])
				return false;
		}

		return true;
	}

	bool Matrix4::operator!=(const Matrix4& _rhs) const
	{
		return !(*this == _rhs);
	}

	Matrix4 Matrix4::operator*(const Matrix4& _rhs) const
	{
		return
		{
			{
				data[0][0] * _rhs[0][0] + data[0][1] * _rhs[1][0] + data[0][2] * _rhs[2][0] + data[0][3] * _rhs[3][0],
				data[0][0] * _rhs[0][1] + data[0][1] * _rhs[1][1] + data[0][2] * _rhs[2][1] + data[0][3] * _rhs[3][1],
				data[0][0] * _rhs[0][2] + data[0][1] * _rhs[1][2] + data[0][2] * _rhs[2][2] + data[0][3] * _rhs[3][2],
				data[0][0] * _rhs[0][3] + data[0][1] * _rhs[1][3] + data[0][2] * _rhs[2][3] + data[0][3] * _rhs[3][3]
			},
			{
				data[1][0] * _rhs[0][0] + data[1][1] * _rhs[1][0] + data[1][2] * _rhs[2][0] + data[1][3] * _rhs[3][0],
				data[1][0] * _rhs[0][1] + data[1][1] * _rhs[1][1] + data[1][2] * _rhs[2][1] + data[1][3] * _rhs[3][1],
				data[1][0] * _rhs[0][2] + data[1][1] * _rhs[1][2] + data[1][2] * _rhs[2][2] + data[1][3] * _rhs[3][2],
				data[1][0] * _rhs[0][2] + data[1][1] * _rhs[1][3] + data[1][2] * _rhs[2][2] + data[1][3] * _rhs[3][2]
			},
			{
				data[2][0] * _rhs[0][0] + data[2][1] * _rhs[1][0] + data[2][2] * _rhs[2][0] + data[2][3] * _rhs[3][0],
				data[2][0] * _rhs[0][1] + data[2][1] * _rhs[1][1] + data[2][2] * _rhs[2][1] + data[2][3] * _rhs[3][1],
				data[2][0] * _rhs[0][2] + data[2][1] * _rhs[1][2] + data[2][2] * _rhs[2][2] + data[2][3] * _rhs[3][2],
				data[2][0] * _rhs[0][3] + data[2][1] * _rhs[1][3] + data[2][2] * _rhs[2][3] + data[2][3] * _rhs[3][2]
			},
			{
				data[3][0] * _rhs[0][0] + data[3][1] * _rhs[1][0] + data[3][2] * _rhs[2][0] + data[3][3] * _rhs[3][0],
				data[3][0] * _rhs[0][1] + data[3][1] * _rhs[1][1] + data[3][2] * _rhs[2][1] + data[3][3] * _rhs[3][1],
				data[3][0] * _rhs[0][2] + data[3][1] * _rhs[1][2] + data[3][2] * _rhs[2][2] + data[3][3] * _rhs[3][2],
				data[3][0] * _rhs[0][3] + data[3][1] * _rhs[1][3] + data[3][2] * _rhs[2][3] + data[3][3] * _rhs[3][3]
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

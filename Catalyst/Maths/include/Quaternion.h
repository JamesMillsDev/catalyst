//  ***************************************************************
//  Quaternion - Creation date: 6/18/2024 3:33:23 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"

#include <string>
#include <initializer_list>

#include "Vector3.h"

using std::string;
using std::initializer_list;

constexpr int QUAT_SIZE = 4;

namespace Catalyst
{
	class Matrix3;
	class Matrix4;

	class DLL Quaternion
	{
	public:
		union
		{
			struct  // NOLINT(clang-diagnostic-nested-anon-types)
			{
				float x;
				float y;
				float z;
				float w;
			};

			float data[QUAT_SIZE];
		};

	public:
		static Quaternion Identity();

		static float Dot(const Quaternion& _a, const Quaternion& _b);
		static Quaternion LookRotation(const Vector3& _viewDir, const Vector3& _forward, const Vector3& _up);
		static Quaternion AxisAngle(const Vector3& _axis, float _angle);

		static Quaternion From(float _pitch, float _yaw, float _roll);
		static Quaternion From(const Vector3& _euler);
		static Quaternion From(const Matrix3& _matrix);
		static Quaternion From(const Matrix4& _matrix);

	public:
		Quaternion();
		Quaternion(float _x, float _y, float _z, float _w);
		// ReSharper disable once CppNonExplicitConvertingConstructor
		Quaternion(const Vector3& _euler);
		Quaternion(const Matrix3& _matrix);
		Quaternion(const Matrix4& _matrix);
		Quaternion(float _pitch, float _yaw, float _roll);

		~Quaternion();

		Quaternion(const Quaternion& _other);
		Quaternion(Quaternion&& _other) noexcept;

	public:
		[[nodiscard]] Vector3 Euler() const;
		void EulerAngles(const Vector3& _euler);
		void EulerAngles(float _pitch, float _yaw, float _roll);

		[[nodiscard]] string ToString() const;

	public:
		bool operator ==(const Quaternion& _rhs) const;
		bool operator !=(const Quaternion& _rhs) const;

		Quaternion operator*(const Quaternion& _rhs) const;
		Quaternion operator*(const Vector3& _rhs) const;

		float operator[](int _index) const;

		Quaternion& operator=(const Quaternion& _other);
		Quaternion& operator=(Quaternion&& _rhs) noexcept;

	};
}
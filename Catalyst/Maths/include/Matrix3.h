//  ***************************************************************
//  Matrix3 - Creation date: 6/18/2024 3:23:48 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"

#include "Vector3.h"

namespace Catalyst
{
	class Quaternion;

	class DLL Matrix3
	{
	public:
		union
		{
			struct  // NOLINT(clang-diagnostic-nested-anon-types)
			{
				// x = 11 y = 12 z = 13
				Vector3 xAxis;
				// x = 21 y = 22 z = 23
				Vector3 yAxis;
				// x = 31 y = 32 z = 33
				Vector3 zAxis;
			};

			float data[VEC_3_SIZE][VEC_3_SIZE];
		};

	public:
		static Matrix3 Identity();

		static Matrix3 MakePitch(float _pitch);
		static Matrix3 MakeYaw(float _yaw);
		static Matrix3 MakeRoll(float _roll);

		static Matrix3 MakeRotation(const Quaternion& _rotation);

		static Matrix3 MakeRotation(float _pitch, float _yaw, float _roll);
		static Matrix3 MakeRotation(const Vector3& _euler);

	public:
		Matrix3();
		Matrix3(const Quaternion& _quat);
		Matrix3(Vector3 _xAxis, Vector3 _yAxis, Vector3 _zAxis);
		Matrix3(float _11, float _12, float _13, 
			float _21, float _22, float _23, 
			float _31, float _32, float _33);
		~Matrix3();

		Matrix3(const Matrix3& _other);
		Matrix3(Matrix3&&) = delete;

	public:
		void SetPitch(float _pitch);
		[[nodiscard]] float Pitch() const;

		void SetYaw(float _yaw);
		[[nodiscard]] float Yaw() const;

		void SetRoll(float _roll);
		[[nodiscard]] float Roll() const;

		void SetRotation(float _pitch, float _yaw, float _roll);
		void SetRotation(const Vector3& _euler);
		void SetRotation(const Quaternion& _quat);

		[[nodiscard]] Quaternion Rotation() const;
		[[nodiscard]] Vector3 Euler() const;
		void Rotation(float* _pitch, float* _yaw, float* _roll) const;

		Matrix3 Transposed();

		[[nodiscard]] string ToString() const;

	public:
		bool operator==(const Matrix3& _rhs) const;
		bool operator!=(const Matrix3& _rhs) const;

		Matrix3 operator*(const Matrix3& _rhs) const;
		Vector3 operator*(const Vector3& _rhs) const;

		Vector3 operator[](int _index) const;

		Matrix3& operator=(const Matrix3& _rhs);
		Matrix3& operator=(Matrix3&& _rhs) noexcept;

	protected:

	};
}
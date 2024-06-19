//  ***************************************************************
//  Matrix4 - Creation date: 6/18/2024 3:30:26 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"

#include "Vector4.h"

namespace Catalyst
{
	class Matrix3;
	class Quaternion;

	class DLL Matrix4
	{
	public:
		union
		{
			struct  // NOLINT(clang-diagnostic-nested-anon-types)
			{
				Vector4 xAxis;
				Vector4 yAxis;
				Vector4 zAxis;
				Vector4 wAxis;
			};

			float data[VEC_4_SIZE][VEC_4_SIZE];
		};

	public:
		Matrix4();
		// ReSharper disable once CppNonExplicitConvertingConstructor
		Matrix4(const Matrix3& _rhs);
		Matrix4(const Quaternion& _quat);
		Matrix4(Vector4 _xAxis, Vector4 _yAxis, Vector4 _zAxis, Vector4 _wAxis);
		Matrix4(float _11, float _12, float _13, float _14, 
			float _21, float _22, float _23, float _24, 
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44);

		~Matrix4();

		Matrix4(const Matrix4& _other);
		Matrix4(Matrix4&&) = delete;

	public:
		bool operator==(const Matrix4& _rhs) const;
		bool operator!=(const Matrix4& _rhs) const;

		Matrix4 operator*(const Matrix4& _rhs) const;
		Vector4 operator*(const Vector4& _rhs) const;

		Vector4 operator[](int _index) const;

		Matrix4& operator=(const Matrix4& _rhs);
		Matrix4& operator=(Matrix4&& _rhs) noexcept;

	};
}
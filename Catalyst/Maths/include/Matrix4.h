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

		~Matrix4();

		Matrix4(const Matrix4& _other);
		Matrix4(Matrix4&&) = delete;

	public:
		Matrix4& operator=(const Matrix4& _other);
		Matrix4& operator=(Matrix4&&) = delete;

		Vector4& operator[](int _index);

	protected:

	};
}
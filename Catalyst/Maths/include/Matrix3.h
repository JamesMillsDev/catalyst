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
				Vector3 xAxis;
				Vector3 yAxis;
				Vector3 zAxis;
			};

			float data[VEC_3_SIZE][VEC_3_SIZE];
		};

	public:
		Matrix3();
		Matrix3(const Quaternion& _quat);
		~Matrix3();

		Matrix3(const Matrix3& _other);
		Matrix3(Matrix3&&) = delete;

	public:
		Matrix3& operator=(const Matrix3& _rhs);
		Matrix3& operator=(Matrix3&&) = delete;

		Vector3& operator[](int _index);

	protected:

	};
}
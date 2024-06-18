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

namespace Catalyst
{
	class DLL Matrix3
	{
	public:
		Matrix3();
		~Matrix3();

		Matrix3(const Matrix3& _other);
		Matrix3(Matrix3&&) = delete;

	public:
		Matrix3& operator=(const Matrix3& _rhs);
		Matrix3& operator=(Matrix3&&) = delete;

	protected:

	};
}
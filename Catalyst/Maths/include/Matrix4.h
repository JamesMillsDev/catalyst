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

namespace Catalyst
{
	class DLL Matrix4
	{
	public:
		Matrix4();
		~Matrix4();

		Matrix4(const Matrix4& _other);
		Matrix4(Matrix4&&) = delete;

	public:
		Matrix4& operator=(const Matrix4& _other);
		Matrix4& operator=(Matrix4&&) = delete;

	protected:

	};
}
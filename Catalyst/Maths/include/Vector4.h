//  ***************************************************************
//  Vector4 - Creation date: 6/18/2024 3:31:38 PM
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
	class DLL Vector4
	{
	public:
		Vector4();
		~Vector4();

		Vector4(const Vector4& _other);
		Vector4(Vector4&&) = delete;

	public:
		Vector4& operator=(const Vector4& _other);
		Vector4& operator=(Vector4&&) = delete;

	protected:

	};
}
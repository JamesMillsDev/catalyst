//  ***************************************************************
//  Vector3 - Creation date: 6/18/2024 3:30:54 PM
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
	class DLL Vector3
	{
	public:
		Vector3();
		~Vector3();

		Vector3(const Vector3& _other);
		Vector3(Vector3&&) = delete;

	public:
		Vector3& operator=(const Vector3& _other);
		Vector3& operator=(Vector3&&) = delete;

	protected:

	};
}
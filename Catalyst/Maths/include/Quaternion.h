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

namespace Catalyst
{
	class DLL Quaternion
	{
	public:
		Quaternion();
		~Quaternion();

		Quaternion(const Quaternion& _other);
		Quaternion(Quaternion&&) = delete;

	public:
		Quaternion& operator=(const Quaternion& _other);
		Quaternion& operator=(Quaternion&&) = delete;

	protected:

	};
}
//  ***************************************************************
//  CatalystMath - Creation date: 6/18/2024 3:51:50 PM
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
	class DLL CatalystMath
	{
	public:
		static float pi;
		static float threshold;

	public:
		static bool Compare(float _x, float _y);

		static float Clamp(float _val, float _min, float _max);
		static float Clamp01(float _val);

		static float Radians(float _deg);
		static float Degrees(float _rad);

	};
}
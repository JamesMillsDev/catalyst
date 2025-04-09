//  ***************************************************************
//  Color - Creation date: 6/9/2024 5:27:46 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "CatalystMath.h"

#include <cstdint>
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using glm::vec3;
using glm::vec4;

namespace Catalyst
{
	struct DLL Color
	{
	public:
		float r;
		float g;
		float b;
		float a;

	public:
		Color();
		Color(float _r, float _g, float _b, float _a);

	public:
		operator vec4() const;
		operator vec3() const;
		Color& operator*(float _multi);
		Color& operator*(const Color& _other);

		bool operator==(Color& _other) const;
		bool operator!=(Color& _other) const;

	};
}
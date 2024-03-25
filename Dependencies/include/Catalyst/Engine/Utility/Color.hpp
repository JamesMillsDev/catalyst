#pragma once

#include <Catalyst/Catalyst.hpp>

#include <cstdint>
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using glm::vec3;
using glm::vec4;

namespace Catalyst
{
	struct Color
	{
	public:
		float r;
		float g;
		float b;
		float a;

	public:
		DLL Color();
		DLL Color(float _r, float _g, float _b, float _a);

	public:
		DLL operator vec4() const;
		DLL operator vec3() const;
		DLL Color& operator*(float _multi);
		DLL Color& operator*(const Color& _other);

		DLL bool operator==(Color& _other) const;
		DLL bool operator!=(Color& _other) const;

	};
}


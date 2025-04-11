//  ***************************************************************
//  PhysicsModule - Creation date: 7/9/2024 10:27:35 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "CatalystMath.h"

#include <glm/vec4.hpp>

using glm::vec4;

namespace Catalyst
{
	class DLL Vector4
	{
		friend class Vector2;
		friend class Vector3;

	public:
		union
		{
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};

			vec4 inner;

		};

	public:
		static Vector4 zero;
		static Vector4 one;
		static Vector4 half;
		static Vector4 up;
		static Vector4 down;
		static Vector4 left;
		static Vector4 right;
		static Vector4 forward;
		static Vector4 back;

	public:
		Vector4();
		Vector4(float _scalar);
		Vector4(float _x, float _y, float _z, float _w);
		Vector4(const class Vector3& _vec, float _w);
		Vector4(const vec4& _other);

		Vector4(const Vector4& _other);
		Vector4(Vector4&& _other) noexcept;

		~Vector4();

	public:
		operator vec4() const;

		bool operator==(const Vector4& _rhs) const;
		bool operator!=(const Vector4& _rhs) const;

		Vector4 operator+(const Vector4& _rhs) const;
		Vector4& operator+=(const Vector4& _rhs);

		Vector4 operator-(const Vector4& _rhs) const;
		Vector4& operator-=(const Vector4& _rhs);

		Vector4 operator*(float _rhs) const;
		Vector4& operator*=(float _rhs);

		Vector4& operator=(const Vector4& _other);
		Vector4& operator=(Vector4&& _other) noexcept;

	};
}

//  ***************************************************************
//  PhysicsModule - Creation date: 7/9/2024 10:27:32 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "CatalystMath.h"

#include <glm/vec3.hpp>

using glm::vec3;

namespace Catalyst
{
	class DLL Vector3
	{
		friend class Vector2;
		friend class Vector4;

	public:
		union
		{
			struct
			{
				float x;
				float y;
				float z;
			};

			vec3 inner;

		};

	public:
		static Vector3 zero;
		static Vector3 one;
		static Vector3 half;
		static Vector3 up;
		static Vector3 down;
		static Vector3 left;
		static Vector3 right;
		static Vector3 forward;
		static Vector3 back;

	public:
		Vector3();
		Vector3(float _scalar);
		Vector3(float _x, float _y, float _z);
		Vector3(const class Vector4& _vec);
		Vector3(const vec3& _other);

		Vector3(const Vector3& _other);
		Vector3(Vector3&& _other) noexcept;

		~Vector3();

	public:
		operator vec3() const;

		bool operator==(const Vector3& _rhs) const;
		bool operator!=(const Vector3& _rhs) const;

		Vector3 operator+(const Vector3& _rhs) const;
		Vector3& operator+=(const Vector3& _rhs);

		Vector3 operator-(const Vector3& _rhs) const;
		Vector3& operator-=(const Vector3& _rhs);

		Vector3 operator*(float _rhs) const;
		Vector3& operator*=(float _rhs);

		Vector3& operator=(const Vector3& _other);
		Vector3& operator=(Vector3&& _other) noexcept;

	};
}
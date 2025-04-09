//  ***************************************************************
//  PhysicsModule - Creation date: 7/9/2024 10:27:30 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "CatalystMath.h"

#include <glm/vec2.hpp>

using glm::vec2;

namespace Catalyst
{
	class DLL Vector2
	{
		friend class Vector3;
		friend class Vector4;

	public:
		union
		{
			struct
			{
				float x;
				float y;
			};

			vec2 inner;

		};

	public:
		Vector2();
		Vector2(float _scalar);
		Vector2(float _x, float _y);
		Vector2(const class Vector3& _vec);
		Vector2(const class Vector4& _vec);
		Vector2(const vec2& _other);

		Vector2(const Vector2& _other);
		Vector2(Vector2&& _other) noexcept;

		~Vector2();

	public:
		operator vec2() const;

		bool operator==(const Vector2& _rhs) const;
		bool operator!=(const Vector2& _rhs) const;

		Vector2 operator+(const Vector2& _rhs) const;
		Vector2& operator+=(const Vector2& _rhs);

		Vector2 operator-(const Vector2& _rhs) const;
		Vector2& operator-=(const Vector2& _rhs);

		Vector2 operator*(float _rhs) const;
		Vector2& operator*=(float _rhs);

		Vector2& operator=(const Vector2& _other);
		Vector2& operator=(Vector2&& _other) noexcept;

	};
}

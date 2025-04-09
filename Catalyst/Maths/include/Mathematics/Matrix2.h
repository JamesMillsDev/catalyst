//  ***************************************************************
//  PhysicsModule - Creation date: 7/9/2024 10:53:15 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "CatalystMath.h"

#include <glm/mat2x2.hpp>

using glm::vec2;
using glm::mat2;

namespace Catalyst
{
	class DLL Matrix2
	{
		friend class Matrix3;
		friend class Matrix4;

	public:
		Matrix2();
		Matrix2(float _scalar);
		Matrix2(float _11, float _21,
			float _12, float _22);
		Matrix2(const vec2& _x, const vec2& _y);
		Matrix2(const mat2& _other);

		Matrix2(const Matrix2& _other);
		Matrix2(Matrix2&& _other) noexcept;

		~Matrix2();

	public:
		operator mat2() const;

		bool operator==(const Matrix2& _rhs) const;
		bool operator!=(const Matrix2& _rhs) const;

		Matrix2 operator*(const Matrix2& _rhs) const;

		Matrix2& operator=(const Matrix2& _other);
		Matrix2& operator=(Matrix2&& _other) noexcept;

	private:
		mat2 m_inner;
	};
}

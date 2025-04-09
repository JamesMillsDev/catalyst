//  ***************************************************************
//  PhysicsModule - Creation date: 7/9/2024 10:27:33 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "CatalystMath.h"

#include <glm/mat3x3.hpp>

using glm::vec3;
using glm::mat3;

namespace Catalyst
{
	class DLL Matrix3
	{
		friend class Matrix2;
		friend class Matrix4;

	public:
		Matrix3();
		Matrix3(float _scalar);
		Matrix3(float _11, float _21, float _31,
			float _12, float _22, float _32,
			float _13, float _23, float _33);
		Matrix3(const vec3& _x, const vec3& _y, const vec3& _z);
		Matrix3(const mat3& _other);

		Matrix3(const Matrix3& _other);
		Matrix3(Matrix3&& _other) noexcept;

		~Matrix3();

	public:
		operator mat3() const;

		bool operator==(const Matrix3& _rhs) const;
		bool operator!=(const Matrix3& _rhs) const;

		Matrix3 operator*(const Matrix3& _rhs) const;

		Matrix3& operator=(const Matrix3& _other);
		Matrix3& operator=(Matrix3&& _other) noexcept;

	private:
		mat3 m_inner;
	};
}
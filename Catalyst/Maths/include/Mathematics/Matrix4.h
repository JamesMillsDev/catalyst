//  ***************************************************************
//  PhysicsModule - Creation date: 7/9/2024 10:27:34 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "CatalystMath.h"

#include "Vector3.h"
#include "Quaternion.h"

#include <glm/mat4x4.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

namespace Catalyst
{
	class DLL Matrix4
	{
		friend class Matrix2;
		friend class Matrix3;

	public:
		Matrix4();
		Matrix4(float _scalar);
		Matrix4(float _11, float _21, float _31, float _41,
			float _12, float _22, float _32, float _42,
			float _13, float _23, float _33, float _43,
			float _14, float _24, float _34, float _44);
		Matrix4(const vec4& _x, const vec4& _y, const vec4& _z, const vec4& _w);
		Matrix4(const mat4& _other);

		Matrix4(const Matrix4& _other);
		Matrix4(Matrix4&& _other) noexcept;

		~Matrix4();

	public:
		static Matrix4 MakeTranslation(const Vector3& _trans);
		static Matrix4 MakeScale(const Vector3& _scale);

		static Matrix4 MakeRotation(const Quaternion& _rotation);
		static Matrix4 MakeEuler(const Vector3& _euler);

		static Matrix4 MakeTransform(const Vector3& _trans, const Quaternion& _rot, const Vector3& _scale);
		static Matrix4 MakeTransform(const Vector3& _trans, const Vector3& _euler, const Vector3& _scale);

		static Matrix4 Perspective(float _fov, float _aspect, float _near, float _far);
		static Matrix4 Orthographic(float _left, float _right, float _bottom, float _top);

		static Matrix4 LookAt(const Vector3& _eye, const Vector3& _target, const Vector3& _up);

		static Matrix4 Identity();

	public:
		void LookAt(const Vector3& _target);
		void LookAt(const Vector3& _target, const Vector3& _up);

		void TRS(const Vector3& _trans, const Quaternion& _rot, const Vector3& _scale);
		void TRS(const Vector3& _trans, const Vector3& _euler, const Vector3& _scale);

		[[nodiscard]] Matrix4 Inverse() const;

		[[nodiscard]] Vector3 Forward() const;
		[[nodiscard]] Vector3 Right() const;
		[[nodiscard]] Vector3 Up() const;

		[[nodiscard]] Vector3 Translation() const;

		[[nodiscard]] Vector3 Scale() const;

		[[nodiscard]] Quaternion Rotation() const;
		[[nodiscard]] Vector3 Euler() const;

		void SetTranslation(const Vector3& _trans);

		void SetScale(const Vector3& _scale);

		void SetRotation(const Quaternion& _rot);
		void SetEuler(const Vector3& _euler);

		void Translate(const Vector3& _delta);

		void ScaleBy(const Vector3& _delta);

		void Rotate(const Quaternion& _delta);
		void Rotate(const Vector3& _delta);

	public:
		operator mat4() const;

		bool operator==(const Matrix4& _rhs) const;
		bool operator!=(const Matrix4& _rhs) const;

		Matrix4 operator*(const Matrix4& _rhs) const;

		Matrix4& operator=(const Matrix4& _other);
		Matrix4& operator=(Matrix4&& _other) noexcept;

	private:
		mat4 m_inner;
	};
}
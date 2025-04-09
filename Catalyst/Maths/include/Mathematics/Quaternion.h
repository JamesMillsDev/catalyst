//  ***************************************************************
//  PhysicsModule - Creation date: 7/9/2024 9:51:28 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "CatalystMath.h"

#include <glm/gtc/quaternion.hpp>

using glm::quat;

namespace Catalyst
{
	class DLL Quaternion
	{
	public:
		Quaternion();
		Quaternion(float _x, float _y, float _z, float _w);
		Quaternion(const quat& _other);
		Quaternion(const class Vector3& _euler);

		Quaternion(const Quaternion& _other);
		Quaternion(Quaternion&& _other) noexcept;

		~Quaternion();

	public:
		operator quat() const;

		bool operator==(const Quaternion& _rhs) const;
		bool operator!=(const Quaternion& _rhs) const;

		Quaternion& operator=(const Quaternion& _other);
		Quaternion& operator=(Quaternion&& _other) noexcept;

	private:
		quat m_inner;
	};
}

//  ***************************************************************
//  Quaternion - Creation date: 6/18/2024 3:33:23 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"

#include <string>
#include <initializer_list>

#include "Vector3.h"
#include "Matrix3.h"
#include "Matrix4.h"

using std::string;
using std::initializer_list;

constexpr int QUAT_SIZE = 4;

namespace Catalyst
{
	class DLL Quaternion
	{
	public:
		union
		{
			struct  // NOLINT(clang-diagnostic-nested-anon-types)
			{
				float x;
				float y;
				float z;
				float w;
			};

			float data[QUAT_SIZE];
		};

	public:
		Quaternion();
		Quaternion(float _x, float _y, float _z, float _w);
		// ReSharper disable once CppNonExplicitConvertingConstructor
		Quaternion(const Vector3& _euler);
		Quaternion(Matrix3& _matrix);
		Quaternion(Matrix4& _matrix);
		Quaternion(float _pitch, float _yaw, float _roll);

		~Quaternion();

		Quaternion(const Quaternion& _other);
		Quaternion(Quaternion&& _other) noexcept;

	public:
		[[nodiscard]] Vector3 Euler() const;
		void EulerAngles(const Vector3& _euler);
		void EulerAngles(float _pitch, float _yaw, float _roll);

	public:
		bool operator ==(const Quaternion& _rhs) const;
		bool operator !=(const Quaternion& _rhs) const;

		Quaternion& operator=(const Quaternion& _other);
		Quaternion& operator=(Quaternion&&) = delete;

	protected:

	};
}
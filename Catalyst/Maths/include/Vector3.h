//  ***************************************************************
//  Vector3 - Creation date: 6/18/2024 3:30:54 PM
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

using std::string;
using std::initializer_list;

constexpr int VEC_3_SIZE = 3;

namespace Catalyst
{
	class DLL Vector3
	{
	public:
		union
		{
			struct  // NOLINT(clang-diagnostic-nested-anon-types)
			{
				float x;
				float y;
				float z;
			};

			float data[VEC_3_SIZE];
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
		// ReSharper disable once CppNonExplicitConvertingConstructor
		Vector3(float _scalar);
		Vector3(float _x, float _y, float _z);
		Vector3(initializer_list<float> _values);

		~Vector3();

		Vector3(const Vector3& _other);
		Vector3(Vector3&& _other) noexcept;

	public:
		static Vector3 Normalised(const Vector3& _vec);
		static float Distance(const Vector3& _a, const Vector3& _b);

		static float Dot(const Vector3& _lhs, const Vector3& _rhs);
		static Vector3 Cross(const Vector3& _lhs, const Vector3& _rhs);

		static Vector3 Lerp(const Vector3& _a, const Vector3& _b, float _t);

	public:
		[[nodiscard]] float Dot(const Vector3& _rhs) const;
		[[nodiscard]] Vector3 Cross(const Vector3& _rhs) const;

		[[nodiscard]] float Magnitude() const;
		[[nodiscard]] float MagnitudeSqr() const;

		void Normalise();
		[[nodiscard]] Vector3 Normalised() const;

		[[nodiscard]] string ToString() const;

	public:
		bool operator ==(const Vector3& _rhs) const;
		bool operator !=(const Vector3& _rhs) const;

		Vector3 operator -(const Vector3& _rhs) const;
		Vector3& operator -=(const Vector3& _rhs);

		Vector3 operator +(const Vector3& _rhs) const;
		Vector3& operator +=(const Vector3& _rhs);

		Vector3 operator *(const Vector3& _rhs) const;
		Vector3& operator *=(const Vector3& _rhs);

		Vector3 operator *(float _rhs) const;
		Vector3& operator *=(float _rhs);

		float operator[](int _index) const;

		Vector3& operator=(const Vector3& _rhs);
		Vector3& operator=(Vector3&& _rhs) noexcept;

	protected:

	};
}
//  ***************************************************************
//  Vector4 - Creation date: 6/18/2024 3:31:38 PM
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

constexpr int VEC_4_SIZE = 4;

namespace Catalyst
{
	class DLL Vector4
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

			float data[VEC_4_SIZE];
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
		// ReSharper disable once CppNonExplicitConvertingConstructor
		Vector4(float _scalar);
		Vector4(float _x, float _y, float _z, float _w);
		Vector4(initializer_list<float> _values);

		~Vector4();

		Vector4(const Vector4& _other);
		Vector4(Vector4&& _other) noexcept;

	public:
		static Vector4 Normalised(const Vector4& _vec);
		static float Distance(const Vector4& _a, const Vector4& _b);

		static float Dot(const Vector4& _lhs, const Vector4& _rhs);
		static Vector4 Cross(const Vector4& _lhs, const Vector4& _rhs);

		static Vector4 Lerp(const Vector4& _a, const Vector4& _b, float _t);

	public:
		[[nodiscard]] float Magnitude() const;
		[[nodiscard]] float MagnitudeSqr() const;

		void Normalise();
		[[nodiscard]] Vector4 Normalised() const;

		[[nodiscard]] string ToString() const;

	public:
		bool operator ==(const Vector4& _rhs) const;
		bool operator !=(const Vector4& _rhs) const;

		Vector4 operator -(const Vector4& _rhs) const;
		Vector4& operator -=(const Vector4& _rhs);

		Vector4 operator +(const Vector4& _rhs) const;
		Vector4& operator +=(const Vector4& _rhs);

		Vector4 operator*(const Vector4& _rhs) const;
		Vector4& operator*=(const Vector4& _rhs);

		Vector4 operator *(float _rhs) const;
		Vector4& operator *=(float _rhs);

		float operator[](int _index) const;

		Vector4& operator=(const Vector4& _other);
		Vector4& operator=(Vector4&& _rhs) noexcept;

	protected:

	};
}

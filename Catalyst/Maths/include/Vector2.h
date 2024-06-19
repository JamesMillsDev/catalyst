//  ***************************************************************
//  Vector2 - Creation date: 6/18/2024 3:43:15 PM
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

constexpr int VEC_2_SIZE = 2;

namespace Catalyst
{
	class DLL Vector2
	{
	public:
		union
		{
			struct  // NOLINT(clang-diagnostic-nested-anon-types)
			{
				float x;
				float y;
			};

			float data[VEC_2_SIZE];
		};

	public:
		static Vector2 zero;
		static Vector2 one;
		static Vector2 half;
		static Vector2 up;
		static Vector2 down;
		static Vector2 left;
		static Vector2 right;

	public:
		static Vector2 Normalised(const Vector2& _vec);
		static float Distance(const Vector2& _a, const Vector2& _b);
		static float Dot(const Vector2& _lhs, const Vector2& _rhs);
		static Vector2 Lerp(const Vector2& _a, const Vector2& _b, float _t);

	public:
		Vector2();
		// ReSharper disable once CppNonExplicitConvertingConstructor
		Vector2(float _scalar);
		Vector2(float _x, float _y);
		Vector2(initializer_list<float> _values);

		~Vector2();

		Vector2(const Vector2& _other);
		Vector2(Vector2&& _other) noexcept;

	public:
		[[nodiscard]] float Magnitude() const;
		[[nodiscard]] float MagnitudeSqr() const;

		void Normalise();
		[[nodiscard]] Vector2 Normalised() const;

		[[nodiscard]] string ToString() const;

	public:
		bool operator==(const Vector2& _rhs) const;
		bool operator!=(const Vector2& _rhs) const;

		Vector2 operator-(const Vector2& _rhs) const;
		Vector2& operator-=(const Vector2& _rhs);

		Vector2 operator+(const Vector2& _rhs) const;
		Vector2& operator+=(const Vector2& _rhs);

		Vector2 operator*(const Vector2& _rhs) const;
		Vector2& operator*=(const Vector2& _rhs);

		Vector2 operator*(float _rhs) const;
		Vector2& operator*=(float _rhs);

		Vector2& operator=(const Vector2& _other);
		Vector2& operator=(Vector2&& _rhs) noexcept;

	protected:

	};
}

//  ***************************************************************
//  Matrix4 - Creation date: 6/18/2024 3:30:26 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"

#include "Vector4.h"

namespace Catalyst
{
	class Vector3;
	class Matrix3;
	class Quaternion;

	/* A 4D matrix used for storing 3D transformations. */
	class DLL Matrix4
	{
	public:
		union
		{
			struct  // NOLINT(clang-diagnostic-nested-anon-types)
			{
				/* The x-axis component of the matrix. */
				Vector4 xAxis;
				/* The y-axis component of the matrix. */
				Vector4 yAxis;
				/* The z-axis component of the matrix. */
				Vector4 zAxis;
				/* The w-axis (translation) component of the matrix. */
				Vector4 wAxis;
			};

			/* A complete array of the values in the matrix for easy individual value accessing. */
			float data[VEC_4_SIZE][VEC_4_SIZE];
		};

	public:
		Matrix4();
		// ReSharper disable once CppNonExplicitConvertingConstructor
		Matrix4(const Matrix3& _rhs);
		Matrix4(const Quaternion& _quat);
		Matrix4(Vector4 _xAxis, Vector4 _yAxis, Vector4 _zAxis, Vector4 _wAxis);
		Matrix4(float _11, float _12, float _13, float _14, 
			float _21, float _22, float _23, float _24, 
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44);

		~Matrix4();

		Matrix4(const Matrix4& _other);
		Matrix4(Matrix4&&) = delete;

	public:
		/* Builds an identity matrix (a matrix with a scale of 1, zero rotation and zero translation) */
		static Matrix4 Identity();

		/**
		 * 
		 * @param _x 
		 * @param _y 
		 * @param _z 
		 * @return 
		 */
		static Matrix4 MakeTranslation(float _x, float _y, float _z);
		/**
		 * 
		 * @param _translation 
		 * @return 
		 */
		static Matrix4 MakeTranslation(const Vector3& _translation);

		/**
		 * 
		 * @param _pitch 
		 * @return 
		 */
		static Matrix4 MakePitch(float _pitch);
		/**
		 * 
		 * @param _yaw 
		 * @return 
		 */
		static Matrix4 MakeYaw(float _yaw);
		/**
		 * 
		 * @param _roll 
		 * @return 
		 */
		static Matrix4 MakeRoll(float _roll);

		/**
		 * 
		 * @param _rotation 
		 * @return 
		 */
		static Matrix4 MakeRotation(const Quaternion& _rotation);
		/**
		 * 
		 * @param _pitch 
		 * @param _yaw 
		 * @param _roll 
		 * @return 
		 */
		static Matrix4 MakeRotation(float _pitch, float _yaw, float _roll);
		/**
		 * 
		 * @param _euler 
		 * @return 
		 */
		static Matrix4 MakeRotation(const Vector3& _euler);

		/**
		 * 
		 * @param _x 
		 * @param _y 
		 * @param _z 
		 * @return 
		 */
		static Matrix4 MakeScale(float _x, float _y, float _z);
		/**
		 * 
		 * @param _scale 
		 * @return 
		 */
		static Matrix4 MakeScale(const Vector3& _scale);

	public:
		/**
		 * 
		 * @param _x 
		 * @param _y 
		 * @param _z 
		 */
		void SetTranslation(float _x, float _y, float _z);
		/**
		 * 
		 * @param _translation 
		 */
		void SetTranslation(const Vector3& _translation);

		/**
		 * 
		 * @param _x 
		 * @param _y 
		 * @param _z 
		 */
		void Translate(float _x, float _y, float _z);
		/**
		 * 
		 * @param _translation 
		 */
		void Translate(const Vector3& _translation);

		/**
		 * 
		 * @return 
		 */
		[[nodiscard]] Vector3 Translation() const;
		/**
		 * 
		 * @param _x 
		 * @param _y 
		 * @param _z 
		 */
		void Translation(float* _x, float* _y, float* _z) const;

		/**
		 * 
		 * @param _pitch 
		 */
		void SetPitch(float _pitch);
		/**
		 * 
		 * @return 
		 */
		[[nodiscard]] float Pitch() const;

		/**
		 * 
		 * @param _yaw 
		 */
		void SetYaw(float _yaw);
		/**
		 * 
		 * @return 
		 */
		[[nodiscard]] float Yaw() const;

		/**
		 * 
		 * @param _roll 
		 */
		void SetRoll(float _roll);
		/**
		 * 
		 * @return 
		 */
		[[nodiscard]] float Roll() const;

		/**
		 * 
		 * @param _pitch 
		 * @param _yaw 
		 * @param _roll 
		 */
		void SetRotation(float _pitch, float _yaw, float _roll);
		/**
		 * 
		 * @param _euler 
		 */
		void SetRotation(const Vector3& _euler);
		/**
		 * 
		 * @param _quat 
		 */
		void SetRotation(const Quaternion& _quat);

		/**
		 * 
		 * @return 
		 */
		[[nodiscard]] Quaternion Rotation() const;
		/**
		 * 
		 * @return 
		 */
		[[nodiscard]] Vector3 Euler() const;
		/**
		 * 
		 * @param _pitch 
		 * @param _yaw 
		 * @param _roll 
		 */
		void Rotation(float* _pitch, float* _yaw, float* _roll) const;

		/**
		 * 
		 * @param _x 
		 * @param _y 
		 * @param _z 
		 */
		void SetScale(float _x, float _y, float _z);
		/**
		 * 
		 * @param _scale 
		 */
		void SetScale(const Vector3& _scale);

		/**
		 * Gets the full scale of the matrix in a 3D vector format.
		 * @return The complete scale of the matrix.
		 */
		[[nodiscard]] Vector3 Scale() const;
		/**
		 * Gets the scale of the matrix and assigns the passed pointers to the corresponding axis.
		 * Handles nullptr passing to allow the user to only get a single axis if needed.
		 * @param _x The scale along the X axis
		 * @param _y The scale along the Y axis
		 * @param _z The scale along the Z axis
		 */
		void Scale(float* _x, float* _y, float* _z) const;

		/* Transposes the matrix from one major to another */
		Matrix4 Transposed();

		/* Creates a string representing the values of the matrix. */
		[[nodiscard]] string ToString() const;

	public:
		bool operator==(const Matrix4& _rhs) const;
		bool operator!=(const Matrix4& _rhs) const;

		Matrix4 operator*(const Matrix4& _rhs) const;
		Vector4 operator*(const Vector4& _rhs) const;

		Vector4 operator[](int _index) const;

		Matrix4& operator=(const Matrix4& _rhs);
		Matrix4& operator=(Matrix4&& _rhs) noexcept;

	};
}
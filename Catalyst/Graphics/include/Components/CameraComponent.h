//  ***************************************************************
//  CameraComponent - Creation date: 6/18/2024 10:31:54 AM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "ActorComponent.h"

#include "Mathematics/Matrix4.h"

namespace Catalyst
{
	class DLL CameraComponent final : public ActorComponent
	{
	public:
		GENERATED_COMPONENT(CameraComponent);

	public:
		CameraComponent();
		~CameraComponent() override;

		CameraComponent(const CameraComponent&) = delete;
		CameraComponent(CameraComponent&&) = delete;

	public:
		[[nodiscard]] float Fov() const;
		[[nodiscard]] float Aspect() const;

		Matrix4 ProjectTransform(const Matrix4& _mat);
		Matrix4 ProjectionView();

		void SetFovAngle(float _fovAngle);
		[[nodiscard]] float GetFovAngle() const;

		void SetNearPlane(float _near);
		[[nodiscard]] float GetNearPlane() const;

		void SetFarPlane(float _far);
		[[nodiscard]] float GetFarPlane() const;

	public:
		CameraComponent& operator=(const CameraComponent&) = delete;
		CameraComponent& operator=(CameraComponent&&) = delete;

	private:
		mat4 m_viewMat;
		mat4 m_projMat;

		float m_fovAngle;
		float m_near;
		float m_far;

	};

	REGISTER_COMPONENT(CameraComponent);
}

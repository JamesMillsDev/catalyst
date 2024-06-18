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

#include <glm/mat4x4.hpp>

using glm::mat4;

namespace Catalyst
{
	class DLL CameraComponent final : public ActorComponent
	{
	public:
		CameraComponent();
		~CameraComponent() override;

		CameraComponent(const CameraComponent&) = delete;
		CameraComponent(CameraComponent&&) = delete;

	public:
		[[nodiscard]] float Fov() const;
		[[nodiscard]] float Aspect() const;

		mat4 ProjectTransform(const mat4& _mat);
		mat4 ProjectionView();

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
}

//  ***************************************************************
//  ViewportCameraActor - Creation date: 6/18/2024 11:42:59 AM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Actor.h"
#include "Components/CameraComponent.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Actions/InputAction2DAxis.h"

using glm::vec2;
using glm::vec3;

namespace Catalyst
{
	class Config;

	class ViewportCameraActor final : public Actor
	{
		friend class EditorApplication;

	public:
		ViewportCameraActor();
		~ViewportCameraActor() override;

		ViewportCameraActor(const ViewportCameraActor&) = delete;
		ViewportCameraActor(ViewportCameraActor&&) = delete;

	public:
		void Initialise(Config* _config);
		[[nodiscard]] mat4 ProjectionView() const;

	public:
		ViewportCameraActor& operator=(const ViewportCameraActor&) = delete;
		ViewportCameraActor& operator=(ViewportCameraActor&&) = delete;

	protected:
		void Tick() override;

	private:
		CameraComponent* m_camera;
		InputAction2DAxis* m_mouseMovement;

		float m_theta;
		float m_phi;

		float m_turnSpeed;
		float m_moveSpeed;

	};
}

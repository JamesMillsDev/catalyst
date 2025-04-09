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

#include "Actions/InputAction2DAxis.h"
#include "Actions/InputAction1DAxis.h"

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
		[[nodiscard]] Matrix4 ProjectionView() const;

	public:
		ViewportCameraActor& operator=(const ViewportCameraActor&) = delete;
		ViewportCameraActor& operator=(ViewportCameraActor&&) = delete;

	protected:
		void Tick() override;

	private:
		CameraComponent* m_camera;
		InputAction2DAxis* m_mouseMovement;
		InputAction1DAxis* m_mouseScroll;

		float m_theta;
		float m_phi;

		float m_turnSpeed;
		float m_moveSpeed;

		float m_speedFactor;

		bool m_isCursorHidden;

	private:
		static void ConfineMouse();
		static void CenterMouse();

	};
}

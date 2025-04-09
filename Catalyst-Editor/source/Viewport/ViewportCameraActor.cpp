#include "Viewport/ViewportCameraActor.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/ext/scalar_common.hpp>

#include "ActorTransform.h"
#include "CatalystTime.h"
#include "InputModule.h"
#include "Utility/Config.h"

using glm::vec4;

namespace Catalyst
{
	ViewportCameraActor::ViewportCameraActor()
		: m_theta{ 0.f }, m_phi{ 0.f }, m_turnSpeed{ 0.f }, m_moveSpeed{ 0.f }, m_speedFactor{ 1.f },
		m_isCursorHidden{ false }, m_borderSize{ 0 }, m_minMoveSpeed{ 0.f }, m_maxMoveSpeed{ 0.f }
	{
		m_camera = CreateComponent<CameraComponent>();

		if (const ActorTransform* transform = Transform())
		{
			transform->TRS({ 10.f, 0.f, 2.f }, Vector3::zero, Vector3::one);
			transform->LookAt(Vector3::zero);
		}
	}

	ViewportCameraActor::~ViewportCameraActor() = default;

	void ViewportCameraActor::Initialise(Config* _config)
	{
		m_camera->SetFovAngle(_config->GetValue<float>("viewport", "camera.fov"));
		m_camera->SetNearPlane(_config->GetValue<float>("viewport", "camera.near"));
		m_camera->SetFarPlane(_config->GetValue<float>("viewport", "camera.far"));

		m_turnSpeed = glm::radians(_config->GetValue<float>("viewport", "camera.turnSpeed"));
		m_moveSpeed = _config->GetValue<float>("viewport", "camera.moveSpeed");

		m_borderSize = _config->GetValue<int>("viewport", "camera.borderSize");
		m_minMoveSpeed = _config->GetValue<float>("viewport", "camera.minMoveSpeed");
		m_maxMoveSpeed = _config->GetValue<float>("viewport", "camera.maxMoveSpeed");

		m_mouseMovement = new InputAction2DAxis(static_cast<Binding>(EAxis::MouseDelta));
		InputModule::AddAction("Editor.MouseMove", m_mouseMovement);

		m_mouseScroll = new InputAction1DAxis(static_cast<Binding>(EAxis::MouseScroll));
		InputModule::AddAction("Editor.MouseSpeedDial", m_mouseScroll);
	}

	Matrix4 ViewportCameraActor::ProjectionView() const
	{
		return m_camera->ProjectionView();
	}

	void ViewportCameraActor::Tick()
	{
		Actor::Tick();

		auto context = glfwGetCurrentContext();

		if (!InputModule::IsButtonDown(static_cast<Binding>(EMouseButton::Right)))
		{
			if (m_isCursorHidden)
			{
				CenterMouse();
				glfwSetInputMode(context, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				m_isCursorHidden = false;

				if (glfwRawMouseMotionSupported())
				{
					glfwSetInputMode(context, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
				}
			}

			return;
		}

		if (const ActorTransform* transform = Transform())
		{
			if (!m_isCursorHidden)
			{
				glfwSetInputMode(context, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				m_isCursorHidden = true;

				if (glfwRawMouseMotionSupported())
					glfwSetInputMode(context, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
			}

			Vector3 movement = Vector3::zero;

			const float dt = CatalystTime::DeltaTime();

			// We will use WASD to move and the Q & E to go up and down
			if (InputModule::IsButtonDown(static_cast<Binding>(EKey::W)))
			{
				movement += Vector3::forward * dt * m_moveSpeed;
			}

			if (InputModule::IsButtonDown(static_cast<Binding>(EKey::S)))
			{
				movement -= Vector3::forward * dt * m_moveSpeed;
			}

			if (InputModule::IsButtonDown(static_cast<Binding>(EKey::A)))
			{
				movement -= Vector3::right * dt * m_moveSpeed;
			}

			if (InputModule::IsButtonDown(static_cast<Binding>(EKey::D)))
			{
				movement += Vector3::right * dt * m_moveSpeed;
			}

			if (InputModule::IsButtonDown(static_cast<Binding>(EKey::Q)))
			{
				movement -= Vector3::up * dt * m_moveSpeed;
			}

			if (InputModule::IsButtonDown(static_cast<Binding>(EKey::E)))
			{
				movement += Vector3::up * dt * m_moveSpeed;
			}

			// Get the mouse coordinates
			const Vector2 mouse = m_mouseMovement->ReadValue();

			m_theta += m_turnSpeed * mouse.x * dt * -1.f;
			m_phi += m_turnSpeed * mouse.y * dt;
			m_phi = glm::clamp(m_phi, -89.f, 89.f);

			m_speedFactor += m_mouseScroll->ReadValue() * .1f;
			m_speedFactor = glm::clamp(m_speedFactor, m_minMoveSpeed, m_maxMoveSpeed);

			if (movement != Vector3::zero)
			{
				transform->UpdateLocation(movement * m_speedFactor);
			}

			transform->SetEulerAngles({ 0.f, m_phi, m_theta });

			ConfineMouse();
		}
	}

	void ViewportCameraActor::ConfineMouse()
	{
		const auto context = glfwGetCurrentContext();

		double cursorX, cursorY; // Get mouse position, relative to window
		glfwGetCursorPos(context, &cursorX, &cursorY);

		int width, height;   // Get dimensions of window
		glfwGetWindowSize(context, &width, &height);

		if (cursorX > width - m_borderSize)
		{
			glfwSetCursorPos(context, m_borderSize, cursorY);
		}
		else if (cursorX < m_borderSize)
		{
			glfwSetCursorPos(context, width - m_borderSize, cursorY);
		}
		if (cursorY > height - m_borderSize)
		{
			glfwSetCursorPos(context, cursorX, m_borderSize);
		}
		else if (cursorY < m_borderSize)
		{
			glfwSetCursorPos(context, cursorX, height - m_borderSize);
		}
	}

	void ViewportCameraActor::CenterMouse()
	{
		const auto context = glfwGetCurrentContext();

		int width, height;   // Get dimensions of window
		glfwGetWindowSize(context, &width, &height);

		glfwSetCursorPos(context, static_cast<float>(width) * .5f, static_cast<float>(height) * .5f);
	}
}

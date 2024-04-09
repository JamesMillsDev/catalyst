#include "ViewportCamera.h"

#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <glm/ext.hpp>

#include <Catalyst/Engine/Screen.hpp>
#include <Catalyst/Engine/Time.hpp>
#include <Catalyst/Gameplay/Actors/Transform.hpp>
#include <catalyst/Input/CompositeAction1DAxis.hpp>
#include <catalyst/Input/CompositeAction2DAxis.hpp>
#include <catalyst/Input/InputAction2DAxis.hpp>
#include <catalyst/Input/InputActionButton.hpp>
#include <Catalyst/Input/InputModule.hpp>

using Catalyst::InputModule;

constexpr const char* VP_CAM_MOVE = "VP_MOVE";
constexpr const char* VP_CAM_LOOK = "VP_LOOK";
constexpr const char* VP_CAM_VERTICAL = "VP_VERTICAL";
constexpr const char* VP_CAM_ACTIVATE = "VP_ACTIVATE";

namespace Catalyst
{
	ViewportCamera::ViewportCamera(const shared_ptr<Config>& _config, class Screen* _screen)
		: Camera(_config->GetValue<float>("viewport", "camera.fov"),
			_config->GetValue<float>("viewport", "camera.near"),
			_config->GetValue<float>("viewport", "camera.far")),
		m_screen{ _screen }, m_theta{ 0 }, m_phi{ 0 }, m_lastMouse{ 0, 0 }
	{
		m_moveSpeed = _config->GetValue<float>("viewport", "camera.moveSpeed");
		m_turnSpeed = glm::radians(_config->GetValue<float>("viewport", "camera.turnSpeed"));

		if (InputModule::AddAction(VP_CAM_MOVE, new CompositeAction2DAxis(EKey::W, EKey::S, EKey::D, EKey::A)))
			m_move = InputModule::Find<InputAction2DAxis>(VP_CAM_MOVE);

		if (InputModule::AddAction(VP_CAM_LOOK, new InputAction2DAxis(static_cast<Binding>(EAxis::MouseDelta))))
			m_look = InputModule::Find<InputAction2DAxis>(VP_CAM_LOOK);

		if (InputModule::AddAction(VP_CAM_VERTICAL, new CompositeAction1DAxis(EKey::E, EKey::Q)))
			m_vertical = InputModule::Find<InputAction1DAxis>(VP_CAM_VERTICAL);

		if (InputModule::AddAction(VP_CAM_ACTIVATE, new InputActionButton(static_cast<Binding>(EMouseButton::Right))))
		{
			m_activate = InputModule::Find<InputActionButton>(VP_CAM_ACTIVATE);
			m_activate->AddPerformedListener(this, &ViewportCamera::OnMovePerformed);
			m_activate->AddCompletedListener(this, &ViewportCamera::OnMoveCompleted);
		}
	}

	void ViewportCamera::OnMovePerformed(bool _perf)
	{
	}

	void ViewportCamera::OnMoveCompleted(bool _perf)
	{
	}

	/*void ViewportCamera::OnCameraActivatePerformed(const bool _perf)
	{
		m_isCameraActive = true;

		glfwSetInputMode(m_screen->GetWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void ViewportCamera::OnCameraActivateCompleted(bool _perf)
	{
		m_isCameraActive = false;

		glfwSetInputMode(m_screen->GetWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void ViewportCamera::OnPlanarMovementPerformed(const vec2 _value)
	{
		if (all(lessThan(glm::abs(_value), vec2(glm::epsilon<float>()))))
			return;

		const vec3 offset = Forward() * _value.y + Right() * _value.x;
		m_transform->UpdateLocation(offset * Time::DeltaTime() * m_moveSpeed);
	}

	void ViewportCamera::OnVerticalMovementPerformed(const float _value)
	{
		if (glm::abs(_value) < glm::epsilon<float>())
			return;

		const vec3 offset = vec3{ 0.f, 1.f, 0.f } *_value;
		m_transform->UpdateLocation(offset * Time::DeltaTime() * m_moveSpeed);
	}

	void ViewportCamera::OnCameraLookPerformed(const vec2 _value)
	{
		if (all(lessThan(glm::abs(_value), vec2(glm::epsilon<float>()))))
			return;

		const vec2 lookInput = _value * Time::DeltaTime() * m_turnSpeed;

		m_theta += lookInput.x;
		m_phi += lookInput.y;
		m_phi = glm::clamp(m_phi, -89.f, 89.f);

		quat rot = quat(m_theta, vec3(0, 1, 0));
		rot *= quat(m_phi, vec3(1, 0, 0));

		m_transform->SetRotation(rot);
	}*/
}

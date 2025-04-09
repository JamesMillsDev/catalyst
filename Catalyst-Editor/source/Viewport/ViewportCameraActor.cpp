#include "Viewport/ViewportCameraActor.h"

#include <iostream>
#include <GLFW/glfw3.h>

#include "ActorTransform.h"
#include "CatalystTime.h"
#include "InputModule.h"
#include "Utility/Config.h"

namespace Catalyst
{
	ViewportCameraActor::ViewportCameraActor()
	{
		m_camera = CreateComponent<CameraComponent>();

		if (const ActorTransform* transform = Transform())
		{
			transform->TRS({ 0, 2.f, -10.f }, vec3(0), vec3(1));
			transform->LookAt(vec3(0));
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

		m_mouseMovement = new InputAction2DAxis(static_cast<Binding>(EAxis::MouseDelta));
		InputModule::AddAction("Editor.MouseMove", m_mouseMovement);
	}

	mat4 ViewportCameraActor::ProjectionView() const
	{
		return m_camera->ProjectionView();
	}

	void ViewportCameraActor::Tick()
	{
		if (!InputModule::IsButtonDown(static_cast<Binding>(EMouseButton::Right)))
			return;

		vec3 movement = vec3(0);
		const float thetaR = glm::radians(m_theta);
		const float phiR = glm::radians(m_phi);

		vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR),
			glm::cos(phiR) * glm::sin(thetaR));
		vec3 right(-glm::sin(thetaR), 0, glm::cos(thetaR));
		vec3 up(0, 1, 0);

		const float dt = CatalystTime::DeltaTime();

		// We will use WASD to move and the Q & E to go up and down
		if (InputModule::IsButtonDown(static_cast<Binding>(EKey::W)))
			movement += forward * dt * m_moveSpeed;
		if (InputModule::IsButtonDown(static_cast<Binding>(EKey::S)))
			movement -= forward * dt * m_moveSpeed;
		if (InputModule::IsButtonDown(static_cast<Binding>(EKey::A)))
			movement -= right * dt * m_moveSpeed;
		if (InputModule::IsButtonDown(static_cast<Binding>(EKey::D)))
			movement += right * dt * m_moveSpeed;

		if (InputModule::IsButtonDown(static_cast<Binding>(EKey::Q)))
			movement -= up * dt * m_moveSpeed;
		if (InputModule::IsButtonDown(static_cast<Binding>(EKey::E)))
			movement += up * dt * m_moveSpeed;

		// Get the mouse coordinates
		const vec2 mouse = m_mouseMovement->ReadValue();

		m_theta += m_turnSpeed * mouse.x * dt;
		m_phi += m_turnSpeed * mouse.y * dt;

		std::cout << m_theta << ":" << m_phi << ":" << m_turnSpeed << "\n";

		if (const ActorTransform* transform = Transform())
		{
			if(movement != vec3(0))
				transform->UpdateLocation(movement);
			transform->SetRotation(quat({ glm::radians(m_phi), glm::radians(m_theta), 0 }));
		}
	}
}

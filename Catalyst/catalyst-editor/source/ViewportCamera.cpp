#include "ViewportCamera.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <Catalyst/Input/InputModule.hpp>
#include <GLFW/glfw3.h>

#include "Catalyst/Engine/Time.hpp"
#include "Catalyst/Gameplay/Actors/Transform.hpp"

using Catalyst::InputModule;

namespace Catalyst
{
	ViewportCamera::ViewportCamera(const shared_ptr<Config>& _config)
		: Camera(_config->GetValue<float>("viewport", "camera.fov"),
			_config->GetValue<float>("viewport", "camera.near"),
			_config->GetValue<float>("viewport", "camera.far")),
		m_theta{ 0 }, m_phi{ 0 }, m_turnSpeed{ glm::radians(180.f) }, m_moveSpeed{ 5.f },
		m_lastMouse{ 0, 0 }
	{
	}

	void ViewportCamera::Tick()
	{
		// If the right button is held down, allow camera control
		const InputModule* input = InputModule::GetInstance();
		if (!input->IsMouseButtonDown(RightMouseButton))
			return;

		const float dt = Time::DeltaTime();  // NOLINT(cppcoreguidelines-narrowing-conversions, clang-diagnostic-implicit-float-conversion)

		vec3 position = Location();

		// We will use WASD to move and the Q & E to go up and down
		if (input->IsKeyDown(W))
			position += Forward() * dt * m_moveSpeed;
		if (input->IsKeyDown(S))
			position -= Forward() * dt * m_moveSpeed;
		if (input->IsKeyDown(A))
			position -= Right() * dt * m_moveSpeed;
		if (input->IsKeyDown(D))
			position += Right() * dt * m_moveSpeed;

		if (input->IsKeyDown(Q))
			position -= Up() * dt * m_moveSpeed;
		if (input->IsKeyDown(E))
			position += Up() * dt * m_moveSpeed;

		// Get the mouse coordinates
		const float mx = static_cast<float>(input->GetMouseX());
		const float my = static_cast<float>(input->GetMouseY());

		m_theta -= m_turnSpeed * (mx - m_lastMouse.x) * dt;
		m_phi -= m_turnSpeed * (my - m_lastMouse.y) * dt;

		m_lastMouse = vec2(mx, my);

		m_transform->SetLocation(position);
		m_transform->SetRotation({ m_phi, m_theta, 0.f });
	}
}

#pragma once

#include <Catalyst/Graphics/Camera.hpp>
#include <Catalyst/Engine/Utility/Config.hpp>

#include <glm/vec2.hpp>

using Catalyst::Camera;
using Catalyst::Config;

using glm::vec2;

namespace Catalyst
{
	class ViewportCamera : public Camera
	{
	public:
		ViewportCamera(const shared_ptr<Config>& _config);

	public:
		void Tick();

	private:
		float m_theta;
		float m_phi;

		float m_turnSpeed;
		float m_moveSpeed;

		vec2 m_lastMouse;

	};
}


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
		ViewportCamera(const shared_ptr<Config>& _config, class Screen* _screen);

	public:
		void Tick();

	private:
		class Screen* m_screen;

		float m_theta;
		float m_phi;

		float m_turnSpeed;
		float m_moveSpeed;

		vec2 m_lastMouse;

		class InputAction2DAxis* m_look;
		class InputAction2DAxis* m_move;
		class InputAction1DAxis* m_vertical;
		class InputActionButton* m_activate;

	private:
		DLL void OnMovePerformed(bool _perf);
		DLL void OnMoveCompleted(bool _perf);

	};
}


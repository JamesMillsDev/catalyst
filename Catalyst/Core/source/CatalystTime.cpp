#include "cacorepch.h"
#include "CatalystTime.h"

#include <GLFW/glfw3.h>

namespace Catalyst
{
	float CatalystTime::m_currTime;
	float CatalystTime::m_prevTime;

	float CatalystTime::m_deltaTime;
	uint CatalystTime::m_frames;
	float CatalystTime::m_fpsInterval;
	uint CatalystTime::m_fps;

	float CatalystTime::DeltaTime()
	{
		return m_deltaTime;
	}

	float CatalystTime::AppTime()
	{
		return m_currTime;
	}

	uint CatalystTime::FPS()
	{
		return m_fps;
	}

	void CatalystTime::Init()
	{
		m_prevTime = static_cast<float>(glfwGetTime());
		m_currTime = 0;
		m_deltaTime = 0;
		m_frames = 0;
		m_fpsInterval = 0;
	}

	bool CatalystTime::Tick()
	{
		// Update delta time
		m_currTime = static_cast<float>(glfwGetTime());
		m_deltaTime = m_currTime - m_prevTime;
		if (m_deltaTime > 0.1f)
			m_deltaTime = 0.1f;

		m_prevTime = m_currTime;

		if (glfwGetWindowAttrib(glfwGetCurrentContext(), GLFW_ICONIFIED) != 0)
			return false;

		// Update fps every second
		m_frames++;
		m_fpsInterval += m_deltaTime;
		if (m_fpsInterval >= 1.0f)
		{
			m_fps = m_frames;
			m_frames = 0;
			m_fpsInterval -= 1.0f;
		}

		return true;
	}
}
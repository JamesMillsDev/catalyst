#include <Catalyst/Engine/Time.hpp>

#include <GLFW/glfw3.h>

namespace Catalyst
{
	float Time::m_currTime;
	float Time::m_prevTime;

	float Time::m_deltaTime;
	uint32 Time::m_frames;
	float Time::m_fpsInterval;
	uint32 Time::m_fps;

	float Time::DeltaTime()
	{
		return m_deltaTime;
	}

	float Time::AppTime()
	{
		return m_currTime;
	}

	uint32 Time::FPS()
	{
		return m_fps;
	}

	void Time::Init()
	{
		m_prevTime = static_cast<float>(glfwGetTime());
		m_currTime = 0;
		m_deltaTime = 0;
		m_frames = 0;
		m_fpsInterval = 0;
	}

	bool Time::Tick()
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

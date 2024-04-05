#pragma once

#include <Catalyst/Catalyst.hpp>

namespace Catalyst
{
	class Time
	{
		friend class Application;

	public:
		DLL static float DeltaTime();
		DLL static float AppTime();
		DLL static uint32 FPS();

	private:
		DLL static float m_currTime;
		DLL static float m_prevTime;

		DLL static float m_deltaTime;
		DLL static uint32 m_frames;
		DLL static float m_fpsInterval;
		DLL static uint32 m_fps;

	private:
		DLL static void Init();
		DLL static bool Tick();

	};
}

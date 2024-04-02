#pragma once

#include <Catalyst/Catalyst.hpp>

namespace Catalyst
{
	class Time
	{
		friend class Application;

	public:
		DLL static double DeltaTime();
		DLL static double AppTime();
		DLL static uint32 FPS();

	private:
		DLL static double m_currTime;
		DLL static double m_prevTime;

		DLL static double m_deltaTime;
		DLL static uint32 m_frames;
		DLL static double m_fpsInterval;
		DLL static uint32 m_fps;

	private:
		DLL static void Init();
		DLL static bool Tick();

	};
}

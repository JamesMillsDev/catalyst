//  ***************************************************************
//  Time - Creation date: 6/9/2024 5:24:34 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"

namespace Catalyst
{
	class DLL CatalystTime
	{
		friend class Application;

	public:
		static float DeltaTime();
		static float AppTime();
		static uint FPS();

	private:
		static float m_currTime;
		static float m_prevTime;

		static float m_deltaTime;
		static uint m_frames;
		static float m_fpsInterval;
		static uint m_fps;

	private:
		static void Init();
		static bool Tick();

	};
}
#pragma once

#include <Catalyst/Catalyst.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Catalyst
{
	class Screen
	{
	private:
		friend class BaseApplication;

	private:
		GLFWwindow* m_window;
		uint32 m_width = 1920;
		uint32 m_height = 1080;

	private:
		DLL Screen();
		DLL ~Screen();

	private:
		DLL int Open();
		DLL void Close();

		DLL void BeginFrame();
		DLL void EndFrame();

		DLL GLFWwindow*& Context();

	};
}
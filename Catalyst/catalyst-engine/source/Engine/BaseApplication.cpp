#include <Catalyst/Engine/BaseApplication.hpp>

#include <Catalyst/Engine/Screen.hpp>

#include <GLFW/glfw3.h>

namespace Catalyst
{
	shared_ptr<BaseApplication> BaseApplication::m_appInstance;

	const char* BaseApplication::GetApplicationDirectory()
	{
		return m_appInstance->m_appPath.c_str();
	}
	
	void BaseApplication::GetWindowSize(int* _w, int* _h)
	{
		if (!m_appInstance->m_manualWindowSize)
		{
			glfwGetWindowSize(m_appInstance->m_screen->Context(), _w, _h);
		}
		else
		{
			*_w = m_appInstance->m_windowWidth;
			*_h = m_appInstance->m_windowHeight;
		}
	}

	GLFWwindow* BaseApplication::GetWindow()
	{
		return m_appInstance->m_screen->Context();
	}
	
	BaseApplication::BaseApplication(GameInstance* _game, const string& _appPath)
		: m_game{ _game }, m_appPath{ std::move(_appPath) },
		m_screen{ new Screen }, m_manualWindowSize{ false }, m_windowWidth{ 0 },
		m_windowHeight{ 0 }
	{
	}
	
	void BaseApplication::OnApplicationOpened() { }

	void BaseApplication::OnApplicationClosed() { }

	void BaseApplication::Tick() { }

	void BaseApplication::Render() { }

	int BaseApplication::Process()
	{
		if (m_screen->Open() == GLFW_FALSE)
			return GLFW_FALSE;

		InitManagers();
		OnApplicationOpened();

		if (m_game != nullptr)
			m_game->OnGameLoaded();

		while (!glfwWindowShouldClose(m_screen->Context()))
		{
			glfwPollEvents();

			Tick();

			if (m_game != nullptr)
				m_game->Tick();


			/* Begin rendering the frame */
			m_screen->BeginFrame();

			Render();

			m_screen->EndFrame();
		}

		m_screen->Close();

		OnApplicationClosed();
		CleanupManagers();

		return GLFW_TRUE;
	}

	void BaseApplication::InitManagers()
	{

	}

	void BaseApplication::CleanupManagers()
	{

	}
}
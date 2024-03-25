#include <Catalyst/Engine/BaseApplication.hpp>

#include <Catalyst/Engine/Screen.hpp>
#include <Catalyst/Engine/Utility/Config.hpp>

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

	BaseApplication::BaseApplication(GameInstance* _game, string _appPath)
		: m_game{ _game }, m_screen{ new Screen }, m_config{ std::make_shared<Config>() }, 
		m_appPath{ std::move(_appPath) }, m_windowWidth{ 0 }, m_windowHeight{ 0 },
		m_manualWindowSize{ false }
	{
	}

	BaseApplication::~BaseApplication()
	{
		delete m_game;
		m_game = nullptr;

		delete m_screen;
		m_screen = nullptr;
	}

	void BaseApplication::OnApplicationOpened() { }

	void BaseApplication::OnApplicationClosed() { }

	void BaseApplication::Tick() { }

	void BaseApplication::Render() { }

	int BaseApplication::Process()
	{
		m_config->Load();

		if (m_screen->Open(m_config) == GLFW_FALSE)
			return GLFW_FALSE;

		InitManagers();
		OnApplicationOpened();

		if (m_game != nullptr)
			m_game->OnGameLoaded();

		while (!glfwWindowShouldClose(m_screen->Context()))
		{
			/* Begin rendering the frame */
			if(!m_screen->BeginFrame())
				continue;

			Tick();

			if (m_game != nullptr)
				m_game->Tick();

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

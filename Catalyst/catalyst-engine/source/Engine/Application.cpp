#include <Catalyst/Engine/Application.hpp>

#include <Catalyst/Engine/Screen.hpp>
#include <Catalyst/Engine/Utility/Config.hpp>

#include <GLFW/glfw3.h>

#include "Catalyst/Engine/Time.hpp"
#include "Catalyst/Graphics/Graphics.hpp"
#include "Catalyst/Gameplay/GameplayModule.hpp"
#include "Catalyst/Input/InputModule.hpp"

namespace Catalyst
{
	shared_ptr<Application> Application::m_appInstance;

	const char* Application::GetApplicationDirectory()
	{
		return m_appInstance->m_appPath.c_str();
	}

	void Application::GetWindowSize(int* _w, int* _h)
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

	GLFWwindow* Application::GetWindow()
	{
		return m_appInstance->m_screen->Context();
	}

	Application::Application(GameInstance* _game, string _appPath)
		: m_game{ _game }, m_screen{ new Screen }, m_config{ std::make_shared<Config>() }, 
		m_appPath{ std::move(_appPath) }, m_windowWidth{ 0 }, m_windowHeight{ 0 },
		m_manualWindowSize{ false }
	{
	}

	Application::~Application()
	{
		delete m_game;
		m_game = nullptr;

		delete m_screen;
		m_screen = nullptr;
	}

	void Application::OnApplicationOpened() { }

	void Application::OnApplicationClosed() { }

	void Application::Tick() { }

	void Application::Render() { }

	int Application::Process()
	{
		m_config->Load();

		if (m_screen->Open(m_config) == GLFW_FALSE)
			return GLFW_FALSE;

		InitModules();
		OnApplicationOpened();

		if (m_game != nullptr)
			m_game->OnGameLoaded();

		Time::Init();

		while (!glfwWindowShouldClose(m_screen->Context()))
		{
			if(!Time::Tick())
				continue;

			/* Begin rendering the frame */
			if(!m_screen->BeginFrame())
				continue;

			for (const auto& module : m_modules)
				module->Tick(this);

			Tick();

			if (m_game != nullptr)
				m_game->Tick();

			for (const auto& module : m_modules)
				module->Render(this);

			Render();

			m_screen->EndFrame();
		}

		m_screen->Close();

		OnApplicationClosed();
		CleanupModules();

		return GLFW_TRUE;
	}

	void Application::InitModules()
	{
		// Create module instances here
		m_modules.emplace_back(new Graphics);
		m_modules.emplace_back(new GameplayModule);
		m_modules.emplace_back(new InputModule);

		// end module creation

		for (const auto& module : m_modules)
			module->OnInitialise(this);
	}

	void Application::CleanupModules()
	{
		for (auto& module : m_modules)
		{
			module->OnShutdown(this);
			delete module;
			module = nullptr;
		}

		m_modules.clear();
	}
}

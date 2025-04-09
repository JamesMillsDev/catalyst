#include "cacorepch.h"
#include "Application.h"

#include <iostream>

#include "CatalystTime.h"
#include "Screen.h"
#include "Utility/Config.h"

namespace Catalyst
{
	Application* Application::m_app = nullptr;

	Application::Application()
		: m_screen{ new Screen }, m_config{ new Config }
	{
		
	}

	Application::~Application()
	{
		for(const auto& module : m_modules)
		{
			module->Exit();
			delete module;
		}

		m_modules.clear();

		delete m_screen;
		m_screen = nullptr;

		delete m_config;
		m_config = nullptr;
	}

	int Application::Run()
	{
		if(m_app == nullptr)
			return EXIT_FAILURE;

		return m_app->Process();
	}

	Screen* Application::GetScreen()
	{
		assert(m_app);

		return m_app->m_screen;
	}

	void Application::OnOpened() { }

	void Application::OnClosed() { }

	void Application::Tick() { }

	void Application::Render() { }

	int Application::Process()
	{
		m_config->Load();

		m_screen->Open(m_config);

		CatalystTime::Init();

		while(!m_screen->WindowShouldClose())
		{
			CatalystTime::Tick();

			m_screen->BeginFrame();

			Tick();
			for(const auto& module : m_modules)
				module->Tick();

			Render();
			for(const auto& module : m_modules)
				module->Render();

			m_screen->EndFrame();
		}

		m_screen->Close();

		return EXIT_SUCCESS;
	}
}

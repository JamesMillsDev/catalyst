#include "cacorepch.h"
#include "Application.h"

#include <iostream>

#include "CatalystTime.h"
#include "Screen.h"
#include "EngineConfig.h"

#include "Utility/Config.h"

namespace Catalyst
{
	Application* Application::m_app = nullptr;

	Application::Application()
		: m_screen{ new Screen }, m_config{ nullptr }
	{
		
	}

	Application::~Application()
	{
		for(auto& module : m_modules)
		{
			module->Exit();
			delete module;
			module = nullptr;
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

	void Application::AssignInstance(Application* _app)
	{
		// To handle the incoming being an editor
		if(m_app)
		{
			_app->m_modules = m_app->m_modules;
			_app->m_screen = m_app->m_screen;
			_app->m_config = m_app->m_config;

			m_app->m_modules.clear();

			m_app->m_screen = nullptr;
			m_app->m_config = nullptr;

			delete m_app;
		}

		m_app = _app;
	}

	Application* Application::GetApp()
	{
		return m_app;
	}

	void Application::OnOpened() { }

	void Application::OnClosed() { }

	void Application::Tick() { }

	void Application::Render() { }

	void Application::GenerateConfigFiles()
	{
		m_config = new Config(embedded_file, embedded_file_size);
	}

	void Application::TickModules()
	{
		for (const auto& module : m_modules)
		{
			module->Tick();
		}
	}

	int Application::Process()
	{
		GenerateConfigFiles();

		m_config->Load();

		m_screen->Open(m_config);

		CatalystTime::Init();

		for (const auto& module : m_modules)
			module->BeginPlay();

		OnOpened();

		while(!m_screen->WindowShouldClose())
		{
			CatalystTime::Tick();

			Tick();
			TickModules();

			m_screen->BeginFrame();

			for(const auto& module : m_modules)
			{
				module->Render();
			}

			Render();

			m_screen->EndFrame();
		}

		OnClosed();

		for (const auto& module : m_modules)
			module->EndPlay();

		m_screen->Close();

		return EXIT_SUCCESS;
	}
}

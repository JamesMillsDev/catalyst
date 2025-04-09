//  ***************************************************************
//  Application - Creation date: 6/7/2024 1:38:02 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"
#include "IModule.h"

#include <list>

using std::list;

namespace Catalyst
{
	class Screen;
	class Config;

	class DLL Application
	{
	public:
		Application();

		Application(const Application&) = delete;
		Application(Application&&) = delete;

		virtual ~Application();

	public:
		static int Run();

		template<derived<IModule> MODULE>
		static MODULE* GetModule();

		static Screen* GetScreen();

	public:
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

	protected:
		static void AssignInstance(Application* _app);
		static Application* GetApp();

	protected:
		virtual void OnOpened();
		virtual void OnClosed();

		virtual void Tick();
		virtual void Render();

		virtual void GenerateConfigFiles();

	private:
		friend static void IModule::AddModule(IModule* _module);
		friend void MakeApplicationInstance();
		friend void DestroyApplicationInstance();

	private:
		static Application* m_app;

	private:
		list<IModule*> m_modules;

		Screen* m_screen;
		Config* m_config;

	private:
		int Process();

	};

	template <derived<IModule> MODULE>
	MODULE* Application::GetModule()
	{
		assert(GetApp());

		for(auto& module : GetApp()->m_modules)
		{
			if(MODULE* m = dynamic_cast<MODULE*>(module))
				return m;
		}

		return nullptr;
	}
}

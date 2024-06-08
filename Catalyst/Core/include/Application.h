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
	class DLL Application
	{
	public:
		Application();

		Application(const Application&) = delete;
		Application(Application&&) = delete;

		virtual ~Application();

	public:
		static int Run();

	public:
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

	private:
		static Application* m_app;

	private:
		list<IModule*> m_modules;

	private:
		friend static void IModule::AddModule(IModule* _module);
		friend void MakeApplicationInstance();

	};
}

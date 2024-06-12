//  ***************************************************************
//  Application - Creation date: 6/7/2024 2:51:02 PM
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
	class DLL IModule
	{
		friend class Application;

	public:
		virtual ~IModule();

	public:
		static void AddModule(IModule* _module);

	protected:
		virtual void Enter() = 0;
		virtual void Exit() = 0;

		virtual void Tick();
		virtual void Render();

	};
}
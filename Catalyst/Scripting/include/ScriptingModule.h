//  ***************************************************************
//  ScriptingModule - Creation date: 6/8/2024 2:38:00 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "IModule.h"

namespace Catalyst
{
	class DLL ScriptingModule final : public IModule
	{
	public:
		ScriptingModule();

		ScriptingModule(const ScriptingModule&) = delete;
		ScriptingModule(ScriptingModule&&) = delete;

		~ScriptingModule() override;

	public:
		ScriptingModule& operator=(const ScriptingModule&) = delete;
		ScriptingModule& operator=(ScriptingModule&&) = delete;

	public:
		void Enter() override;
		void Exit() override;

	};
}

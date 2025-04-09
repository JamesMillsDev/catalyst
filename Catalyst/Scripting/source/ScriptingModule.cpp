#include "cascriptingpch.h"
#include "ScriptingModule.h"

#include <iostream>

namespace Catalyst
{
	ScriptingModule::ScriptingModule()
	{
		AddModule(this);
	}

	ScriptingModule::~ScriptingModule()
	{
	}

	void ScriptingModule::Enter()
	{
		std::cout << "Entered Scripting module\n";
	}

	void ScriptingModule::Exit()
	{
	}
}

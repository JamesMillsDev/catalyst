#include "cainputpch.h"
#include "InputModule.h"

#include <iostream>

namespace Catalyst
{
	InputModule::InputModule()
	{
		AddModule(this);
	}

	InputModule::~InputModule()
	{
	}

	void InputModule::Enter()
	{
		std::cout << "Entered Input module\n";
	}

	void InputModule::Exit()
	{
	}
}

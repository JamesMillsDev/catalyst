#include "cacorepch.h"
#include "CoreModule.h"

#include <iostream>

namespace Catalyst
{
	CoreModule::CoreModule()
	{
		AddModule(this);
	}

	CoreModule::~CoreModule()
	{
	}

	void CoreModule::Enter()
	{
		std::cout << "Entered Core module\n";
	}

	void CoreModule::Exit()
	{
	}
}

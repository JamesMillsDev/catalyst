#include "cagraphicspch.h"
#include "GraphicsModule.h"

#include <iostream>

namespace Catalyst
{
	GraphicsModule::GraphicsModule()
	{
		AddModule(this);
	}

	GraphicsModule::~GraphicsModule()
	{
	}

	void GraphicsModule::Enter()
	{
		std::cout << "Entered Graphics module\n";
	}

	void GraphicsModule::Exit()
	{
	}
}

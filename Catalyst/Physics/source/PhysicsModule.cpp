#include "caphysicspch.h"
#include "PhysicsModule.h"

#include <iostream>

namespace Catalyst
{
	PhysicsModule::PhysicsModule()
	{
		AddModule(this);
	}

	PhysicsModule::~PhysicsModule()
	{
	}

	void PhysicsModule::Enter()
	{
		std::cout << "Entered Physics module\n";
	}

	void PhysicsModule::Exit()
	{
	}
}

#include "cagameplaypch.h"
#include "GameplayModule.h"

#include <iostream>

namespace Catalyst
{
    GameplayModule::GameplayModule()
    {
		AddModule(this);
    }

    GameplayModule::~GameplayModule()
    {
    }

    void GameplayModule::Enter()
    {
		std::cout << "Entered Gameplay module\n";
    }

    void GameplayModule::Exit()
    {
    }
}

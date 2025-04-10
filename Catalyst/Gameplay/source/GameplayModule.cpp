#include "cagameplaypch.h"
#include "GameplayModule.h"

#include <iostream>

#include "Actor.h"
#include "ActorComponent.h"

namespace Catalyst
{
    GameplayModule::GameplayModule()
    {
		AddModule(this);
    }

    GameplayModule::~GameplayModule()
    {
        for (const auto& actor : m_actors)
            delete actor;

        m_actors.clear();
    }

    void GameplayModule::Enter()
    {
		std::cout << "Entered Gameplay module\n";
    }

    void GameplayModule::Exit()
    {
    }

    void GameplayModule::Tick()
    {
        for (auto& [fnc, obj] : m_changes)
            std::invoke(fnc, this, obj);

        m_changes.clear();

        for (const auto& actor : m_actors)
            actor->ApplyChanges();

        for(const auto& actor : m_actors)
        {
            actor->TickComponents();
            actor->Tick();
        }
    }

    void GameplayModule::Render()
    {
        for (const auto& actor : m_actors)
        {
            actor->RenderComponents();
            actor->Render();
        }
    }

    void GameplayModule::AddActor(Actor* _actor)
    {
        m_actors.emplace_back(_actor);
        _actor->OnBeginPlay();

        for (const auto& comp : _actor->m_components)
            comp->OnBeginPlay();
    }

    void GameplayModule::RemoveActor(Actor* _actor)
    {
        m_actors.remove(_actor);
        _actor->OnEndPlay();

        for (const auto& comp : _actor->m_components)
            comp->OnEndPlay();

        delete _actor;
    }
}

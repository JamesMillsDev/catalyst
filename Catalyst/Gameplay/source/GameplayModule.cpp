#include "cagameplaypch.h"
#include "GameplayModule.h"

#include <iostream>

#include "Actor.h"
#include "ActorComponent.h"
#include "ActorTransform.h"

namespace Catalyst
{
    GameplayModule::GameplayModule()
    {
		AddModule(this);
    }

    GameplayModule::~GameplayModule()
    {
        
    }

    void GameplayModule::RegisterActor(Actor* _actor)
    {
        m_changes.emplace_back(ActorAction::Add, _actor);
    }

    void GameplayModule::Enter()
    {
		std::cout << "Entered Gameplay module\n";
    }

    void GameplayModule::Exit()
    {
        for (const auto& actor : m_actors)
            delete actor;

        m_actors.clear();
    }

    void GameplayModule::Tick()
    {
        for (auto& [fnc, obj] : m_changes)
        {
            switch (fnc)
            {
            case ActorAction::Add:
                AddActor(obj);
                break;
            case ActorAction::Remove:
                RemoveActor(obj);
                break;
            }
        }

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

        std::cout << "Components: " << _actor->m_components.size() << "\n";
        std::cout << "LinkedList address: " << &_actor->m_components << "\n";
        auto& list = _actor->m_components;
        std::cout << "Head: " << list.front() << "\n";
        std::cout << "Tail: " << list.back() << "\n";
        std::cout << "Count: " << list.size() << "\n";

        for (auto it = _actor->m_components.begin(); it != _actor->m_components.end(); ++it)
        {
            //std::cout << "Iterating: " << it.node << "\n";
            if (*it)
                std::cout << "Component ptr: " << *it << "\n";
        }
    }

    void GameplayModule::RemoveActor(Actor* _actor)
    {
        m_actors.remove(_actor);
        _actor->OnEndPlay();

        for (const auto& comp : _actor->m_components)
            comp->OnEndPlay();

        if (_actor->Transform()->Parent() != nullptr)
        {
            _actor->Transform()->SetParent(nullptr);
            _actor->ApplyChanges();
        }

        delete _actor;
    }
}

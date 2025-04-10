#include "cagameplaypch.h"
#include "GameplayModule.h"

#include <iostream>

#include "Actor.h"
#include "ActorComponent.h"
#include "ActorTransform.h"

namespace Catalyst
{
    list<tuple<string, string, GameplayModule::ComponentRegistry::FactoryFunction>> GameplayModule::ComponentRegistry::m_registry;

    void GameplayModule::ComponentRegistry::Register(const string& name, FactoryFunction func)
    {
        if (std::ranges::find_if(m_registry, [name](const auto& comp) { return std::get<0>(comp) == name; }) == m_registry.end())
        {
            string displayName;
            for (size_t i = 0; i < name.find("Component"); ++i)
            {
                if (i > 0 && std::isupper(name[i]))
                {
                    displayName += ' ';
                }
                displayName += name[i];
            }

            m_registry.emplace_back(name, displayName, func);
        }
    }

    list<tuple<string, string, GameplayModule::ComponentRegistry::FactoryFunction>>& GameplayModule::ComponentRegistry::GetRegistry()
    {
        return m_registry;
    }

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
        for (const auto& actor : m_actors)
            delete actor;

        m_actors.clear();
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

<<<<<<< HEAD
        if (_actor->Transform()->Parent() != nullptr)
        {
            _actor->Transform()->SetParent(nullptr);
            _actor->ApplyChanges();
        }

=======
>>>>>>> develop
        delete _actor;
    }
}

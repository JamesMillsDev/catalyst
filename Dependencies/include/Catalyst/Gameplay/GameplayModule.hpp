#pragma once

#include <Catalyst/Catalyst.hpp>
#include <Catalyst/Engine/IModule.hpp>
#include <Catalyst/Gameplay/Actors/Actor.hpp>

#include <list>

using std::list;
using std::pair;

namespace Catalyst
{
	class GameplayModule final : public IModule
	{
		typedef void(GameplayModule::* ActorListChange)(Actor*);

	public:
		DLL GameplayModule();
		DLL ~GameplayModule() override;

		template<Derived<Actor> ACTOR>
		ACTOR* SpawnActor();
		template<Derived<Actor> ACTOR>
		void DestroyActor(ACTOR*& _actor);

	protected:
		DLL void Tick(Application* _app) override;
		DLL void Render(Application* _app) override;

	private:
		list<pair<ActorListChange, Actor*>> m_actorListChanges;

		list<Actor*> m_actors;

	private:
		DLL void AddActor(Actor* _actor);
		DLL void RemoveActor(Actor* _actor);

	};

	template <Derived<Actor> ACTOR>
	ACTOR* GameplayModule::SpawnActor()
	{
		ACTOR* actor = new ACTOR;

		m_actorListChanges.emplace_back(std::make_pair(&GameplayModule::AddActor, actor));
		return actor;
	}

	template <Derived<Actor> ACTOR>
	void GameplayModule::DestroyActor(ACTOR*& _actor)
	{
		m_actorListChanges.emplace_back(std::make_pair(&GameplayModule::RemoveActor, _actor));
	}
}

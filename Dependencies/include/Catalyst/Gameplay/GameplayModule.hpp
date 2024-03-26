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
		typedef void(GameplayModule::* ActorListChange)(shared_ptr<Actor>&);

	public:
		DLL GameplayModule();
		DLL ~GameplayModule() override;

		template<Derived<Actor> ACTOR>
		shared_ptr<ACTOR> SpawnActor();
		template<Derived<Actor> ACTOR>
		void DestroyActor(shared_ptr<ACTOR>& _actor);

	protected:
		DLL void Tick(BaseApplication* _app) override;
		DLL void Render(BaseApplication* _app) override;

	private:
		list<pair<ActorListChange, shared_ptr<Actor>&>> m_actorListChanges;

		list<shared_ptr<Actor>> m_actors;

	private:
		DLL void AddActor(shared_ptr<Actor>& _actor);
		DLL void RemoveActor(shared_ptr<Actor>& _actor);

	};

	template <Derived<Actor> ACTOR>
	shared_ptr<ACTOR> GameplayModule::SpawnActor()
	{
		shared_ptr<ACTOR> actor = std::make_shared<ACTOR>();

		m_actorListChanges.emplace_back(&GameplayModule::AddActor, actor);
		return actor;
	}

	template <Derived<Actor> ACTOR>
	void GameplayModule::DestroyActor(shared_ptr<ACTOR>& _actor)
	{
	}
}

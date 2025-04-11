//  ***************************************************************
//  GameplayModule - Creation date: 6/12/2024 11:10:35 AM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "IModule.h"

#include <functional>
#include <list>
#include <string>
#include <tuple>

using std::function;
using std::list;
using std::pair;
using std::string;
using std::tuple;

namespace Catalyst
{
	class Actor;
	class ActorComponent;

	enum class ActorAction { Add, Remove };

	class DLL GameplayModule final : public IModule
	{
		typedef void(GameplayModule::* ActorListChange)(Actor*);

#if IS_EDITOR
		friend class HierarchyWindow;
#endif

	public:
		GameplayModule();
		~GameplayModule() override;

	public:
		GameplayModule(const GameplayModule&) = delete;
		GameplayModule(GameplayModule&&) = delete;

	public:
		template<derived<Actor> ACTOR>
		ACTOR* SpawnActor();
		void RegisterActor(Actor* _actor);
		template<derived<Actor> ACTOR>
		void DestroyActor(ACTOR* _actor);

	public:
		GameplayModule& operator=(const GameplayModule&) = delete;
		GameplayModule& operator=(GameplayModule&&) = delete;

	protected:
		void Enter() override;
		void Exit() override;

		void Tick() override;
		void Render() override;

	private:
		list<pair<ActorAction, Actor*>> m_changes;
		list<Actor*> m_actors;

	private:
		void AddActor(Actor* _actor);
		void RemoveActor(Actor* _actor);

	};

	template <derived<Actor> ACTOR>
	inline ACTOR* GameplayModule::SpawnActor()
	{
		ACTOR* actor = new ACTOR;

		return actor;
	}

	template <derived<Actor> ACTOR>
	inline void GameplayModule::DestroyActor(ACTOR* _actor)
	{
		m_changes.emplace_back(ActorAction::Remove, _actor);
	}
}

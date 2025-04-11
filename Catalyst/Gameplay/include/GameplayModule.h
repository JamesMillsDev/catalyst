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

	class DLL GameplayModule final : public IModule
	{
		typedef void(GameplayModule::* ActorListChange)(Actor*);

#if IS_EDITOR
		friend class HierarchyWindow;
#endif

	public:
		struct DLL ComponentRegistry
		{
		public:
			using FactoryFunction = function<ActorComponent* ()>;

		public:
			static void Register(const string& name, FactoryFunction func);
			static list<tuple<string, string, FactoryFunction>>& GetRegistry();

		private:
			static list<tuple<string, string, FactoryFunction>> m_registry;

		};

	public:
		GameplayModule();
		~GameplayModule() override;

	public:
		GameplayModule(const GameplayModule&) = delete;
		GameplayModule(GameplayModule&&) = delete;

	public:
		template<derived<Actor> ACTOR>
		ACTOR* SpawnActor();
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
		list<pair<ActorListChange, Actor*>> m_changes;
		list<Actor*> m_actors;

	private:
		void AddActor(Actor* _actor);
		void RemoveActor(Actor* _actor);

	};

	template <derived<Actor> ACTOR>
	ACTOR* GameplayModule::SpawnActor()
	{
		ACTOR* actor = new ACTOR;

		m_changes.emplace_back(&GameplayModule::AddActor, actor);

		return actor;
	}

	template <derived<Actor> ACTOR>
	void GameplayModule::DestroyActor(ACTOR* _actor)
	{
		m_changes.emplace_back(&GameplayModule::RemoveActor, _actor);
	}
}

#pragma once

#include <Catalyst/Catalyst.hpp>
#include <Catalyst/Engine/Object.hpp>
#include <Catalyst/Gameplay/Actors/ActorComponent.hpp>

#include <list>

using std::list;
using std::pair;

namespace Catalyst
{
	class Actor : public Object
	{
		friend class GameplayModule;

		typedef void(Actor::* ComponentListChange)(shared_ptr<ActorComponent>&);

	protected:
		DLL Actor();
		DLL ~Actor() override;

	protected:
		DLL virtual void OnBeginPlay();
		DLL virtual void OnEndPlay();

		DLL virtual void Tick();
		DLL virtual void Render();

		template<Derived<ActorComponent> COMPONENT>
		shared_ptr<COMPONENT>& CreateComponent();
		template<Derived<ActorComponent> COMPONENT>
		void DestroyComponent(shared_ptr<COMPONENT>& _component);

	private:
		list<pair<ComponentListChange, shared_ptr<ActorComponent>&>> m_componentListChanges;

		list<shared_ptr<ActorComponent>> m_components;

	private:
		DLL void AddComponent(shared_ptr<ActorComponent>& _component);
		DLL void RemoveComponent(shared_ptr<ActorComponent>& _component);

	};

	template <Derived<ActorComponent> COMPONENT>
	shared_ptr<COMPONENT>& Actor::CreateComponent()
	{
		shared_ptr<COMPONENT> component = std::make_shared<COMPONENT>();

		m_componentListChanges.emplace_back(&Actor::CreateComponent, component);
		return component;
	}

	template <Derived<ActorComponent> COMPONENT>
	void Actor::DestroyComponent(shared_ptr<COMPONENT>& _component)
	{
		if (std::ranges::find(m_components, _component) == m_components.end())
			return;

		m_componentListChanges.emplace_back(&Actor::RemoveComponent, _component);
	}
}

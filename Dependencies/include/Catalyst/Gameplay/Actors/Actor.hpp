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

		typedef void(Actor::* ComponentListChange)(ActorComponent*);

	public:
		DLL class Transform* GetTransform();
		template<Derived<ActorComponent> COMPONENT>
		COMPONENT* FindComponent();

		template<Derived<ActorComponent> COMPONENT>
		COMPONENT* CreateComponent();
		template<Derived<ActorComponent> COMPONENT>
		void DestroyComponent(COMPONENT* _component);

	protected:
		DLL Actor();
		DLL ~Actor() override;

	protected:
		DLL virtual void OnBeginPlay();
		DLL virtual void OnEndPlay();

		DLL virtual void Tick();
		DLL virtual void Render();

	private:
		list<pair<ComponentListChange, ActorComponent*>> m_componentListChanges;

		class Transform* m_transform;
		list<ActorComponent*> m_components;

	private:
		DLL void AddComponent(ActorComponent* _component);
		DLL void RemoveComponent(ActorComponent* _component);

	};

	template <Derived<ActorComponent> COMPONENT>
	COMPONENT* Actor::FindComponent()
	{
		COMPONENT* found = nullptr;

		for(auto& component : m_components)
		{
			if(COMPONENT* comp = dynamic_cast<COMPONENT*>(component))
			{
				found = comp;
			}
		}

		return found;
	}

	template <Derived<ActorComponent> COMPONENT>
	COMPONENT* Actor::CreateComponent()
	{
		COMPONENT* component = new COMPONENT;
		component->m_owner = this;

		m_componentListChanges.emplace_back(&Actor::AddComponent, component);
		return component;
	}

	template <Derived<ActorComponent> COMPONENT>
	void Actor::DestroyComponent(COMPONENT* _component)
	{
		if (std::ranges::find(m_components, _component) == m_components.end())
			return;

		m_componentListChanges.emplace_back(&Actor::RemoveComponent, _component);
	}
}

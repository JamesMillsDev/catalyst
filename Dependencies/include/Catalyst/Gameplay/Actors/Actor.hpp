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

		typedef void(Actor::* ComponentListChange)(shared_ptr<ActorComponent>);

	public:
		DLL shared_ptr<class Transform> GetTransform();
		template<Derived<ActorComponent> COMPONENT>
		shared_ptr<COMPONENT> FindComponent();

		template<Derived<ActorComponent> COMPONENT>
		shared_ptr<COMPONENT> CreateComponent();
		template<Derived<ActorComponent> COMPONENT>
		void DestroyComponent(shared_ptr<COMPONENT> _component);

	protected:
		DLL Actor();
		DLL ~Actor() override;

	protected:
		DLL virtual void OnBeginPlay();
		DLL virtual void OnEndPlay();

		DLL virtual void Tick();
		DLL virtual void Render();

	private:
		list<pair<ComponentListChange, shared_ptr<ActorComponent>>> m_componentListChanges;

		shared_ptr<class Transform> m_transform;
		list<shared_ptr<ActorComponent>> m_components;

	private:
		DLL void AddComponent(shared_ptr<ActorComponent> _component);
		DLL void RemoveComponent(shared_ptr<ActorComponent> _component);

	};

	template <Derived<ActorComponent> COMPONENT>
	shared_ptr<COMPONENT> Actor::FindComponent()
	{
		shared_ptr<COMPONENT> found = nullptr;

		for(auto& component : m_components)
		{
			if(COMPONENT* comp = dynamic_cast<COMPONENT*>(component.get()))
			{
				found = shared_ptr<COMPONENT>(comp);
			}
		}

		return found;
	}

	template <Derived<ActorComponent> COMPONENT>
	shared_ptr<COMPONENT> Actor::CreateComponent()
	{
		shared_ptr<COMPONENT> component = std::make_shared<COMPONENT>();
		component->m_owner = this;

		m_componentListChanges.emplace_back(&Actor::AddComponent, component);
		return component;
	}

	template <Derived<ActorComponent> COMPONENT>
	void Actor::DestroyComponent(shared_ptr<COMPONENT> _component)
	{
		if (std::ranges::find(m_components, _component) == m_components.end())
			return;

		m_componentListChanges.emplace_back(&Actor::RemoveComponent, _component);
	}
}
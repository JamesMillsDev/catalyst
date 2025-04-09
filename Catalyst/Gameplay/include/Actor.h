//  ***************************************************************
//  Actor - Creation date: 6/12/2024 11:13:31 AM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"
#include "Object.h"

#include "Utility/LinkedList.h"

#include <list>
#include <string>

using std::list;
using std::pair;
using std::string;

namespace Catalyst
{
	class ActorComponent;
	class ActorTransform;

	class DLL Actor : public Object
	{
		friend class GameplayModule;

		typedef void(Actor::* ComponentListChange)(ActorComponent*);

#if IS_EDITOR
		friend class HierarchyWindow;
		friend class InspectorWindow;
#endif

	public:
		Actor(const Actor&) = delete;
		Actor(Actor&&) = delete;

	public:
		ActorTransform* Transform() const;

		string GetName() const;
		void SetName(const string& _newName); 

		bool Enabled() const;
		void SetEnabled(bool newState);

		template<derived<ActorComponent> COMPONENT>
		COMPONENT* FindComponent();
		template<derived<ActorComponent> COMPONENT>
		COMPONENT* CreateComponent();
		template<derived<ActorComponent> COMPONENT>
		void DestroyComponent(COMPONENT* _component);

	public:
		Actor& operator=(const Actor&) = delete;
		Actor& operator=(Actor&&) = delete;
		
	protected:
#if IS_EDITOR
		bool m_hideFromHierarchy;
#endif

	protected:
		Actor();
		~Actor() override;

	protected:
		virtual void OnBeginPlay();
		virtual void OnEndPlay();
		virtual void Tick();
		virtual void Render();

	private:
		LinkedList<pair<ComponentListChange, ActorComponent*>> m_changes;
		LinkedList<ActorComponent*> m_components;

		ActorTransform* m_transform;
		string m_name;

		bool m_isEnabled;

	private:
		void AddComponent(ActorComponent* _component);
		void RemoveComponent(ActorComponent* _component);

		void ApplyChanges();
		void TickComponents() const;
		void RenderComponents() const;

	};

	template <derived<ActorComponent> COMPONENT>
	COMPONENT* Actor::FindComponent()
	{
		for(const auto& comp : m_components)
		{
			if (COMPONENT* component = dynamic_cast<COMPONENT*>(comp))
				return component;
		}

		return nullptr;
	}

	template <derived<ActorComponent> COMPONENT>
	COMPONENT* Actor::CreateComponent()
	{
		COMPONENT* component = new COMPONENT;
		component->m_owner = this;

		m_changes.Add({ &Actor::AddComponent, component });

		return component;
	}

	template <derived<ActorComponent> COMPONENT>
	void Actor::DestroyComponent(COMPONENT* _component)
	{
		m_changes.Add({ &Actor::RemoveComponent, _component });
	}
}
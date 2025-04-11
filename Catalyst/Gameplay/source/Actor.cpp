#include "cagameplaypch.h"
#include "Actor.h"

#include "ActorComponent.h"
#include "ActorTransform.h"

#include <algorithm>

namespace Catalyst
{
	ActorTransform* Actor::Transform() const
	{
		return m_transform;
	}

	Actor::Actor() :
#if IS_EDITOR
		m_hideFromHierarchy{ false },
#endif
		m_transform{ new ActorTransform(this) }, m_name{ "Actor" }, m_isEnabled{ true }
	{
		std::cout << "[Actor::Actor] this = " << this << "\n";
	}

	Actor::~Actor()
	{
		delete m_transform;
		m_transform = nullptr;

		for (const auto& comp : m_components)
		{
			delete comp;
		}

		m_components.clear();
	}

	string Actor::GetName() const
	{
		return m_name;
	}

	void Actor::SetName(const string& _newName)
	{
		m_name = _newName;
	}

	bool Actor::Enabled() const
	{
		return m_isEnabled;
	}

	void Actor::SetEnabled(bool newState)
	{
		m_isEnabled = newState;
	}

	void Actor::OnBeginPlay()
	{
	}

	void Actor::OnEndPlay()
	{
	}

	void Actor::Tick()
	{
	}

	void Actor::Render()
	{
	}

	void Actor::AddComponent(ActorComponent* _component)
	{
		m_components.emplace_back(_component);
	}

	void Actor::RemoveComponent(ActorComponent* _component)
	{
		m_components.emplace_back(_component);
	}

	void Actor::ApplyChanges()
	{
		for (auto& [fnc, obj] : m_changes)
		{
			switch (fnc)
			{
			case ComponentAction::Add:
				AddComponent(obj);
				break;
			case ComponentAction::Remove:
				RemoveComponent(obj);
				break;
			}
		}

		m_changes.clear();

		m_transform->ApplyChanges();
	}

	void Actor::TickComponents() const
	{
		if (!m_isEnabled)
		{
			return;
		}

		for (const auto& comp : m_components)
		{
			comp->Tick();
		}
	}

	void Actor::RenderComponents() const
	{
		if (!m_isEnabled)
		{
			return;
		}

		for (const auto& comp : m_components)
		{
			comp->Render();
		}
	}
}

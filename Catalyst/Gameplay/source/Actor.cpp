#include "cagameplaypch.h"
#include "Actor.h"

#include "ActorComponent.h"
#include "ActorTransform.h"

namespace Catalyst
{
	ActorTransform* Actor::Transform() const
	{
		return m_transform;
	}

	Actor::Actor()
		: m_transform{ new ActorTransform }
	{
		
	}

	Actor::~Actor()
	{
		delete m_transform;
		m_transform = nullptr;

		for (const auto& comp : m_components)
			delete comp;

		m_components.clear();
	}

	void Actor::OnBeginPlay() { }

	void Actor::OnEndPlay() { }

	void Actor::Tick() { }

	void Actor::Render() { }

	void Actor::AddComponent(ActorComponent* _component)
	{
		m_components.emplace_back(_component);
	}

	void Actor::RemoveComponent(ActorComponent* _component)
	{
		m_components.remove(_component);
	}

	void Actor::ApplyChanges()
	{
		for (auto& [fnc, obj] : m_changes)
			std::invoke(fnc, this, obj);

		m_changes.clear();

		m_transform->ApplyChanges();
	}

	void Actor::TickComponents() const
	{
		for (const auto& comp : m_components)
			comp->Tick();
	}

	void Actor::RenderComponents() const
	{
		for (const auto& comp : m_components)
			comp->Render();
	}
}

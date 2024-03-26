#include <Catalyst/Gameplay/Actors/Actor.hpp>

#include <Catalyst/Gameplay/Actors/Transform.hpp>

namespace Catalyst
{
	shared_ptr<Transform> Actor::GetTransform()
	{
		return m_transform;
	}

	Actor::Actor()
		: m_transform{ new Transform }
	{
		
	}

	Actor::~Actor()
	{
		Object::~Object();

		for(auto& component : m_components)
			component.reset();

		m_components.clear();
	}

	void Actor::OnBeginPlay()
	{
		for(const auto& component : m_components)
			component->OnBeginPlay();
	}

	void Actor::OnEndPlay()
	{
		for (const auto& component : m_components)
			component->OnEndPlay();
	}

	void Actor::Tick()
	{
		for (auto& [fnc, component] : m_componentListChanges)
			std::invoke(fnc, this, component);

		m_componentListChanges.clear();

		for (const auto& component : m_components)
			component->Tick();
	}

	void Actor::Render()
	{
		for (const auto& component : m_components)
			component->Render();
	}

	void Actor::AddComponent(shared_ptr<ActorComponent> _component)
	{
		m_components.emplace_back(std::move(_component));
	}

	void Actor::RemoveComponent(shared_ptr<ActorComponent> _component)
	{
		_component->OnEndPlay();
		m_components.remove(_component);
		_component.reset();
	}
}

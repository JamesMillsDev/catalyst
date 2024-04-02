#include <Catalyst/Gameplay/Actors/ActorComponent.hpp>

namespace Catalyst
{
	Actor* ActorComponent::GetOwner() const
	{
		return m_owner;
	}

	ActorComponent::ActorComponent() = default;

	ActorComponent::~ActorComponent()
	{
		Object::~Object();
	}

	void ActorComponent::OnBeginPlay() { }

	void ActorComponent::OnEndPlay() { }

	void ActorComponent::Tick() { }

	void ActorComponent::Render() { }
}

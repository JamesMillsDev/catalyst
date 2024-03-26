#include <Catalyst/Gameplay/Actors/ActorComponent.hpp>

namespace Catalyst
{
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

#include "cagameplaypch.h"
#include "ActorComponent.h"

namespace Catalyst
{
	Actor* ActorComponent::Owner() const
	{
		return m_owner;
	}

	ActorComponent::ActorComponent()
		: m_owner{ nullptr }
	{
	}

	ActorComponent::~ActorComponent()
	{
		m_owner = nullptr;
	}

	void ActorComponent::OnBeginPlay() { }

	void ActorComponent::OnEndPlay() { }

	void ActorComponent::Tick() { }

	void ActorComponent::Render() { }
}

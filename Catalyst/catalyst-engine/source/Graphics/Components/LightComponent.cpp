#include <Catalyst/Graphics/Components/LightComponent.hpp>

#include <Catalyst/Gameplay/Actors/Actor.hpp>
#include <Catalyst/Gameplay/Actors/Transform.hpp>

#include "Catalyst/Engine/Application.hpp"
#include "Catalyst/Graphics/Graphics.hpp"

namespace Catalyst
{
	LightComponent::LightComponent()
		: color{ 1.f, 1.f, 1.f, 1.f }, intensity{ 1.f }, direction{ 0 }, directional{ false }
	{
		if(Graphics* graphics = Application::GetModule<Graphics>())
			graphics->Add(this);
	}

	LightComponent::~LightComponent()
	{
		if(Graphics* graphics = Application::GetModule<Graphics>())
			graphics->Remove(this);

		ActorComponent::~ActorComponent();
	}

	void LightComponent::Tick()
	{
		ActorComponent::Tick();

		if(Transform* transform = GetOwner()->GetTransform())
		{
			direction = transform->Forward();
		}
	}

	vec3 LightComponent::RealColor() const
	{
		return vec3{ color.r, color.g, color.b } * intensity;
	}
}

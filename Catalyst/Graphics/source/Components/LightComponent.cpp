#include "cagraphicspch.h"
#include "Components/LightComponent.h"

#include "Actor.h"
#include "ActorTransform.h"

namespace Catalyst
{
	LightComponent::LightComponent()
		: color{ 1.f, 1.f, 1.f }, intensity{ 1.f }, type{ ELightType::Directional }
	{

	}

	LightComponent::~LightComponent() = default;

	vec3 LightComponent::RealColor() const
	{
		return vec3
		{
			color.r,
			color.g,
			color.b
		} * intensity;
	}
}

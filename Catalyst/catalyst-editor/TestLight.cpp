#include "TestLight.h"

#include <Catalyst/Gameplay/Actors/Transform.hpp>
#include <Catalyst/Graphics/Components/LightComponent.hpp>

using Catalyst::Color;
using Catalyst::LightComponent;

TestLight::TestLight()
{
	LightComponent* light = CreateComponent<LightComponent>();
	light->directional = true;
	light->color = Color{ 1.f, 1.f, 1.f, 1.f };

	GetTransform()->TRS({ 0.f, 0.f, 10.f }, vec3{ 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });
}

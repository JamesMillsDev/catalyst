#include "TestPointLightActor.h"

#include <Catalyst/Gameplay/Actors/Transform.hpp>
#include <Catalyst/Graphics/Components/LightComponent.hpp>

using Catalyst::LightComponent;

TestPointLightActor::TestPointLightActor()
{
	m_light = CreateComponent<LightComponent>();
	GetTransform()->TRS({ 1.f, 0, 0.f }, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });

	SetupLight({ 1.f, 0.f, 0.f, 1.f }, 50.f);
}

void TestPointLightActor::SetupLight(const Color _color, const float _intensity) const
{
	m_light->color = _color;
	m_light->intensity = _intensity;
}

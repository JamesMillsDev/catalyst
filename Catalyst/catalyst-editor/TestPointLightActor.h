#pragma once

#include <Catalyst/Gameplay/Actors/Actor.hpp>

#include <Catalyst/Graphics/Components/LightComponent.hpp>

using Catalyst::LightComponent;
using Catalyst::Color;

class TestPointLightActor : public Catalyst::Actor
{
public:
	TestPointLightActor();

public:
	void SetupLight(Color _color, float _intensity) const;

private:
	shared_ptr<Catalyst::LightComponent> m_light;

};
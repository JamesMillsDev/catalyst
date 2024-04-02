#pragma once

#include <Catalyst/Catalyst.hpp>
#include <Catalyst/Gameplay/Actors/ActorComponent.hpp>
#include <Catalyst/Engine/Utility/Color.hpp>

#include <glm/vec3.hpp>

using glm::vec3;

namespace Catalyst
{
	class LightComponent final : public ActorComponent
	{
	public:
		Color color;
		float intensity;
		vec3 direction;

		bool directional;

	public:
		DLL LightComponent();
		DLL ~LightComponent() override;

	public:
		DLL void Tick() override;

		DLL vec3 RealColor() const;

	};
}
//  ***************************************************************
//  LightComponent - Creation date: 6/17/2024 2:52:44 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"
#include "ActorComponent.h"
#include "IRenderFeature.h"
#include "HeaderMacros.h"

#include <glm/vec3.hpp>

using glm::vec3;

namespace Catalyst
{
	enum class ELightType : ubyte
	{
		Directional,
		Spot,
		Point
	};

	class DLL LightComponent final : public ActorComponent, public IRenderFeature
	{
	public:
		REGISTER_COMPONENT(LightComponent);

	public:
		vec3 color;
		float intensity;

		ELightType type;

	public:
		LightComponent();
		~LightComponent() override;

		LightComponent(const LightComponent&) = delete;
		LightComponent(LightComponent&&) = delete;

	public:
		vec3 RealColor() const;

	public:
		LightComponent& operator=(const LightComponent&) = delete;
		LightComponent& operator=(LightComponent&&) = delete;

	};
}

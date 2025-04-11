#pragma once

#include "Editor.h"

#include <Components/LightComponent.h>

namespace Catalyst
{
	class LightComponentEditor : public Editor<LightComponent>
	{
	protected:
		void OnGui(LightComponent* _component) override;
		string Title() override;

	};
}


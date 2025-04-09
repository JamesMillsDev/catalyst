#include "Editors/LightComponentEditor.h"

#include <Components/LightComponent.h>

namespace Catalyst
{
	void LightComponentEditor::OnGui(LightComponent* _component)
	{
		vec3 vector = _component->color;

		if (ImGui::ColorEdit3("Color", &vector.x))
		{
			_component->color.x = vector.x;
			_component->color.y = vector.y;
			_component->color.z = vector.z;
		}

		ImGui::DragFloat("Intensity", &_component->intensity, 0.01f, 0.01f, 10.f);
	}

	string LightComponentEditor::Title()
	{
		return "Light";
	}
}
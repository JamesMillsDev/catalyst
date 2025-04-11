#include "Windows/InspectorWindow.h"

#include "Windows/HierarchyWindow.h"

#include <iostream>

#include <Actor.h>
#include <ActorTransform.h>
#include <Mathematics/Vector3.h>
#include <Mathematics/Quaternion.h>

#include "Debug/Gizmos.h"
#include "Editors/Editor.h"
#include "Editors/StaticMeshComponentEditor.h"
#include "Editors/LightComponentEditor.h"

#include <ComponentRegistry.h>
#include <Components/LightComponent.h>
#include <Components/CameraComponent.h>
#include <Components/StaticMeshComponent.h>

namespace Catalyst
{
	InspectorWindow::InspectorWindow(HierarchyWindow* _hierarchy)
		: EditorWindow{ "Inspector" }, m_hierarchy{ _hierarchy }
	{
		m_guis.emplace_back(new StaticMeshComponentEditor);
		m_guis.emplace_back(new LightComponentEditor);
	}

	InspectorWindow::~InspectorWindow()
	{
		for (auto& gui : m_guis)
		{
			delete gui;
		}

		m_guis.clear();
	}

	void InspectorWindow::OnGui(EditorApplication* app)
	{
		if (m_hierarchy->m_selectedActor != nullptr)
		{
			mat4 matrix = m_hierarchy->m_selectedActor->Transform()->LocalToWorld();

			Gizmos::AddSphere(vec3(0), 0.1f, 4, 4, { 1.f, 1.f, 1.f, 1.f }, &matrix);

			Actor* selectedActor = m_hierarchy->m_selectedActor;
			HierarchyWindow::ActorInfo& actorInfo = m_hierarchy->GetActorInfo(selectedActor);

			// Create a horizontal layout with a checkbox
			ImGui::PushID(selectedActor);

			// Create the checkbox first
			if (ImGui::Checkbox("##ActorToggle", &actorInfo.enabled))
			{
				selectedActor->SetEnabled(actorInfo.enabled);
			}

			// Create an input field for the actor's name
			ImGui::SameLine();
			char buffer[256];
			strncpy_s(buffer, sizeof(buffer), actorInfo.name.c_str(), actorInfo.name.size()); // Copy current name to buffer
			if (ImGui::InputText("##ActorName", buffer, sizeof(buffer)))
			{
				// Update actor's name when it changes
				actorInfo.name = buffer;
				selectedActor->SetName(actorInfo.name);
			}

			ImGui::PopID();

			DoTransformInspector(m_hierarchy->m_selectedActor->Transform());

			for (auto& component : m_hierarchy->m_selectedActor->m_components)
			{
				for (auto& gui : m_guis)
				{
					gui->Render(component);
				}
			}

			if (ImGui::Button("Add Component", ImVec2(-1, 0)))
			{
				ImGui::OpenPopup("AddComponentPopup");
			}

			if (ImGui::BeginPopup("AddComponentPopup"))
			{
				static char searchBuffer[128] = "";  // Search filter
				ImGui::InputText("##Search", searchBuffer, sizeof(searchBuffer));

				// Iterate over the registered components
				for (const auto& [name, displayName, creator] : ComponentRegistry::GetRegistry())
				{
					if (strlen(searchBuffer) == 0 || strstr(displayName.c_str(), searchBuffer))
					{
						if (ImGui::Selectable(displayName.c_str()))
						{
							ActorComponent* comp = creator();
							comp->m_owner = m_hierarchy->m_selectedActor;

							m_hierarchy->m_selectedActor->AddComponent(comp); // Add new component

							ImGui::CloseCurrentPopup();
						}
					}
				}

				ImGui::EndPopup();
			}
		}
	}

	void InspectorWindow::DoTransformInspector(ActorTransform* _transform)
	{
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			Vector3 location = _transform->Location();
			Quaternion rotation = _transform->Rotation();
			Vector3 scale = _transform->Scale();

			if (DoVector3Slider("Location", &location))
			{
				_transform->SetLocation(location);
			}

			if (DoRotationSlider("Rotation", &rotation))
			{
				_transform->SetRotation(rotation);
			}

			if (DoVector3Slider("Scale", &scale))
			{
				_transform->SetScale(scale);
			}
		}
	}

	bool InspectorWindow::DoVector3Slider(const string& _label, Vector3* _vector)
	{
		vec3 vector = *_vector;

		if (ImGui::DragFloat3(_label.c_str(), &vector.x, .1f, -FLT_MAX, FLT_MAX))
		{
			_vector->x = vector.x;
			_vector->y = vector.y;
			_vector->z = vector.z;

			return true;
		}

		return false;
	}

	bool InspectorWindow::DoRotationSlider(const string& _label, Quaternion* _quat)
	{
		vec3 vector = glm::degrees(glm::eulerAngles((quat)*_quat));

		if (ImGui::DragFloat3(_label.c_str(), &vector.x, .1f, -FLT_MAX, FLT_MAX))
		{
			*_quat = quat(glm::radians(vector));

			return true;
		}

		return false;
	}
}
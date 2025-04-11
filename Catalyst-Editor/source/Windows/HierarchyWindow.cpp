#include "Windows/HierarchyWindow.h"

#include <ImGui/imgui.h>

#include <ActorTransform.h>
#include <Application.h>

#include <Components/CameraComponent.h>
#include <Components/LightComponent.h>
#include <Components/StaticMeshComponent.h>

namespace Catalyst
{
	HierarchyWindow::HierarchyWindow()
		: EditorWindow{ "Hierarchy" }, m_selectedActor{ nullptr }, m_editingActor{ nullptr }
	{
		m_module = Application::GetModule<GameplayModule>();
	}

	HierarchyWindow::~HierarchyWindow()
	{
	}

    Actor* HierarchyWindow::GetSelectedActor()
    {
        return m_selectedActor;
    }

	void HierarchyWindow::OnGui(EditorApplication* app)
	{
		m_hasSelectedThisFrame = false;

        // Create a menu bar at the top of the window.
        if (ImGui::BeginMenuBar())
        {
            // Add more elements in the menu bar (example: buttons)
            if (ImGui::BeginMenu("+"))
            {
                if (ImGui::Selectable("Empty Actor"))
                {
                    Actor* newActor = m_module->SpawnActor<Actor>();
                }

                if (ImGui::BeginMenu("Lights"))
                {
                    if (ImGui::Selectable("Directional"))
                    {
                        LightComponent* light = CreateActorWith<LightComponent>("Directional Light");
                        light->type = ELightType::Directional;
                    }

                    if (ImGui::Selectable("Point"))
                    {
                        LightComponent* light = CreateActorWith<LightComponent>("Point Light");
                        light->type = ELightType::Point;
                    }

                    if (ImGui::Selectable("Spot"))
                    {
                        LightComponent* light = CreateActorWith<LightComponent>("Spot Light");
                        light->type = ELightType::Spot;
                    }

                    ImGui::EndMenu();
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

		for (Actor* actor : m_module->m_actors)
		{
			if (actor->Transform()->Parent() == nullptr)
			{
				DisplayActor(actor);
			}
		}
	}

    ImGuiWindowFlags HierarchyWindow::GetWindowFlags()
    {
        return EditorWindow::GetWindowFlags() | ImGuiWindowFlags_MenuBar;
    }

	void HierarchyWindow::DisplayActor(Actor* _actor)
	{
        ActorInfo& actorInfo = GetActorInfo(_actor);

        if (actorInfo.hideFromHierarchy)
        {
            return;
        }

        ImGui::PushID(_actor);

        HandleDeselection();

        bool isSelected = (_actor == m_selectedActor);
        bool isEditing = (_actor == m_editingActor);

        ImGuiTreeNodeFlags flags = GetBaseFlags(!_actor->Transform()->Children().empty());

        float fullWidth = ImGui::GetContentRegionAvail().x;
        bool nodeOpen = false;

        if (isEditing)
        {
            float indentLevel = ImGui::GetCursorPosX();
            EditActorName(_actor, actorInfo, fullWidth, indentLevel);
        }
        else
        {
            ImVec2 selectableSize = ImGui::CalcTextSize(_actor->GetName().c_str());
            selectableSize.x += ImGui::GetStyle().FramePadding.x * 2;
            selectableSize.y += ImGui::GetStyle().FramePadding.y * 2;

            DisplayActorNode(_actor, flags, isSelected, nodeOpen, fullWidth, selectableSize);
        }

        // Start renaming on F2
        if (isSelected && ImGui::IsKeyPressed(ImGuiKey_F2))
        {
            m_editingActor = _actor;
        }

        HandleDragDropReparent(_actor);

        // Render children recursively
        if (nodeOpen)
        {
            for (ActorTransform* childTransform : _actor->Transform()->Children())
            {
                DisplayActor(childTransform->Owner());
            }
            ImGui::TreePop();
        }

        ImGui::PopID();
	}

    void HierarchyWindow::DisplayActorNode(Actor* _actor, ImGuiTreeNodeFlags& _flags, bool _selected, bool& _open, float _fullWidth, ImVec2 _selectableSize)
    {
        // Draw TreeNode with selectable flag
        if (_selected)
        {
            ImGui::GetWindowDrawList()->AddRectFilled(
                ImGui::GetCursorScreenPos(),
                ImVec2(ImGui::GetCursorScreenPos().x + _fullWidth, ImGui::GetCursorScreenPos().y + _selectableSize.y),
                ImGui::GetColorU32(ImGuiCol_Header)
            );
        }

        if (_selected)
        {
            _flags |= ImGuiTreeNodeFlags_Selected;
        }

        // Handle TreeNode with OpenOnArrow for auto-expansion
        _open = ImGui::TreeNodeEx(_actor, _flags, "%s", _actor->GetName().c_str());

        // Right-click context menu
        if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
        {
            ImGui::OpenPopup("ActorContextMenu");
        }

        // Right-click menu
        if (ImGui::BeginPopup("ActorContextMenu"))
        {
            if (ImGui::MenuItem("Rename"))
            {
                // Handle Rename action
                m_editingActor = _actor;
            }

            if (ImGui::MenuItem("Delete"))
            {
                // Handle Delete action
                m_module->DestroyActor(_actor);
            }

            if (ImGui::MenuItem("Duplicate"))
            {
                // Handle Duplicate action
                // Example: m_module->DuplicateActor(_actor);
            }

            ImGui::EndPopup();
        }

        if (ImGui::IsItemClicked())
        {
            m_selectedActor = _actor;
            m_hasSelectedThisFrame = true;
        }
    }

    void HierarchyWindow::EditActorName(Actor* _actor, ActorInfo& _actorInfo, float _fullWidth, float _indentLevel)
    {
        // Handle renaming inside TreeNode
        char buffer[256];
        strncpy_s(buffer, sizeof(buffer), _actorInfo.name.c_str(), _actorInfo.name.size());

        ImGui::SetNextItemWidth(_fullWidth - _indentLevel - 20); // Adjust width

        // Create the input field for renaming the actor
        if (ImGui::InputText("##RenameActor", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            _actorInfo.name = buffer;
            _actor->SetName(_actorInfo.name);
            m_editingActor = nullptr;
        }

        // Cancel renaming with Escape key
        if (!ImGui::IsItemActive() && (ImGui::IsKeyPressed(ImGuiKey_Escape) || ImGui::IsKeyPressed(ImGuiKey_Enter)))
        {
            m_editingActor = nullptr;
        }
    }

    HierarchyWindow::ActorInfo& HierarchyWindow::GetActorInfo(Actor* _actor)
    {
        if (m_actorInfoMap.find(_actor) == m_actorInfoMap.end())
        {
            m_actorInfoMap[_actor] = { _actor };
        }

        return m_actorInfoMap[_actor];
    }

    ImGuiTreeNodeFlags HierarchyWindow::GetBaseFlags(bool _hasChildren)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
        if (!_hasChildren)
        {
            flags |= ImGuiTreeNodeFlags_Leaf;
        }

        // Add the OpenOnArrow flag for handling expansion with the arrow click
        if (_hasChildren)
        {
            flags |= ImGuiTreeNodeFlags_OpenOnArrow;
        }

        return flags;
    }

    void HierarchyWindow::HandleDragDropReparent(Actor* _actor)
    {
        // Start the drag source
        if (ImGui::BeginDragDropSource())
        {
            // Set the drag payload (passing the actor being dragged)
            ImGui::SetDragDropPayload("Actor", &_actor, sizeof(Actor*));
            ImGui::Text("Move: %s", _actor->GetName().c_str());
            ImGui::EndDragDropSource();

            m_isDragging = true;
        }

        // Check if the mouse is hovering over the window (including empty space)
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
        {
            // Check if the mouse is hovering over an actor (tree node)
            bool isHoveringOverActor = false;

            // You can iterate through the actors and check if the mouse is hovering over the tree node
            for (Actor* actor : m_module->m_actors)
            {
                if (actor->Transform()->Parent() == nullptr)
                {
                    // Calculate the position of the actor's tree node
                    ImVec2 treeNodePos = ImGui::GetCursorScreenPos();
                    ImVec2 treeNodeSize = ImGui::CalcTextSize(actor->GetName().c_str());
                    treeNodeSize.x += ImGui::GetStyle().FramePadding.x * 2;
                    treeNodeSize.y += ImGui::GetStyle().FramePadding.y * 2;

                    // Check if the mouse is hovering over the actor's tree node
                    ImVec2 treeNodeMaxPos = ImVec2(treeNodePos.x + treeNodeSize.x, treeNodePos.y + treeNodeSize.y);
                    if (ImGui::IsMouseHoveringRect(treeNodePos, treeNodeMaxPos))
                    {
                        isHoveringOverActor = true;
                        break;  // We found a tree node, stop checking further
                    }
                }
            }

            // If the mouse is not hovering over any actor, unparent the actor (empty space)
            if (!isHoveringOverActor && m_isDragging && ImGui::IsMouseReleased(0))
            {
                if (_actor->Transform()->Parent() != nullptr)
                {
                    _actor->Transform()->SetParent(nullptr);  // Unparent the actor 
                }
                m_isDragging = false;
            }
        }

        // Handle dragging over a valid target (like other tree nodes for reparenting)
        if (ImGui::BeginDragDropTarget() && m_isDragging)
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Actor"))
            {
                Actor* draggedActor = *(Actor**)payload->Data;
                if (draggedActor != _actor && 
                    !draggedActor->Transform()->IsChild(_actor->Transform()) && 
                    draggedActor->Transform()->Parent() != _actor->Transform())
                {
                    draggedActor->Transform()->SetParent(_actor->Transform());
                }
            }
            ImGui::EndDragDropTarget();
        }
    }

    void HierarchyWindow::HandleDeselection()
    {
        // Check if click occurred in empty space (not on any actor node)
        if (ImGui::IsWindowHovered() && !ImGui::IsItemHovered())
        {
            // Clicked in empty space, deselect actor
            if (ImGui::IsMouseClicked(0)) // Left mouse button
            {
                m_selectedActor = nullptr; // Deselect
            }
        }

        if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && ImGui::IsMouseClicked(0) && 
            m_selectedActor != nullptr && !m_hasSelectedThisFrame)
        {
            m_selectedActor = nullptr;
        }
    }

    HierarchyWindow::ActorInfo::ActorInfo()
		: actor{ nullptr }, name{ "" }
	{
	}

    HierarchyWindow::ActorInfo::ActorInfo(Actor* _actor)
		: actor{ _actor }, name{ _actor->GetName() }, hideFromHierarchy{ _actor->m_hideFromHierarchy }
	{
	}
}

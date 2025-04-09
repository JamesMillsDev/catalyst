#include "Editors/StaticMeshComponentEditor.h"

#include <vector>

#include <Rendering/Mesh.h>
#include <Components/StaticMeshComponent.h>

#include "Assets/MeshAsset.h"

namespace Catalyst
{
    //void AssetPicker(const char* label, std::string& selectedAsset, const std::vector<std::string>& assetList)
    //{
    //    // Calculate text height and button height
    //    float textHeight = ImGui::GetTextLineHeight();
    //    float buttonHeight = ImGui::GetFrameHeight();

    //    // Align label vertically with the button
    //    ImGui::AlignTextToFramePadding(); // Ensures vertical alignment
    //    ImGui::Text("%s", label);

    //    // Ensure label and button are on the same line
    //    ImGui::SameLine();

    //    // Button takes up remaining space
    //    float buttonWidth = ImGui::GetContentRegionAvail().x;
    //    if (ImGui::Button(selectedAsset.empty() ? "None" : selectedAsset.c_str(), ImVec2(buttonWidth, buttonHeight)))
    //    {
    //        ImGui::OpenPopup("AssetPickerPopup");
    //    }

    //    // Drag & Drop Target
    //    if (ImGui::BeginDragDropTarget())
    //    {
    //        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET"))
    //        {
    //            std::string* droppedAsset = (std::string*)payload->Data;
    //            selectedAsset = *droppedAsset;
    //        }
    //        ImGui::EndDragDropTarget();
    //    }

    //    // Popup for selecting an mesh
    //    if (ImGui::BeginPopup("AssetPickerPopup"))
    //    {
    //        for (const auto& mesh : assetList)
    //        {
    //            if (ImGui::Selectable(mesh.c_str()))
    //            {
    //                selectedAsset = mesh;
    //            }

    //            // Allow dragging from the list
    //            if (ImGui::BeginDragDropSource())
    //            {
    //                ImGui::SetDragDropPayload("ASSET", &mesh, sizeof(std::string));
    //                ImGui::Text("Dragging: %s", mesh.c_str());
    //                ImGui::EndDragDropSource();
    //            }
    //        }
    //        ImGui::EndPopup();
    //    }
    //}

    bool ObjectField(const char* label, Asset*& mesh)
    {
        ImGui::Text(label); // Label
        ImGui::SameLine();

        ImVec2 size = ImVec2(200, 20); // Adjust size to your UI

        // Button to display the mesh name or "None"
        if (ImGui::Button(mesh ? mesh->name.c_str() : "None", size))
        {
            // Click action (could open an mesh picker)
            return true;
        }

        return false;

        //// Start drag if there's an mesh assigned
        //if (mesh && ImGui::BeginDragDropSource())
        //{
        //    ImGui::SetDragDropPayload("ASSET_PAYLOAD", &mesh, sizeof(Asset*));
        //    ImGui::Text("Dragging: %s", mesh->name.c_str());
        //    ImGui::EndDragDropSource();
        //}

        //// Accept drop (Assign new mesh)
        //if (ImGui::BeginDragDropTarget())
        //{
        //    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_PAYLOAD"))
        //    {
        //        Asset* droppedAsset = *static_cast<Asset**>(payload->Data);
        //        mesh = droppedAsset; // Assign new mesh
        //    }
        //    ImGui::EndDragDropTarget();
        //}
    }

	void StaticMeshComponentEditor::OnGui(StaticMeshComponent* _component)
	{
        //AssetPicker("Test", m_test, { "A", "B", "C" });
        

        //if (ObjectField("Mesh", mesh))
        //{
        //    if (_component->submeshes.empty())
        //    {
        //        //_component->submeshes.emplace_back(Mesh::FromAssimp());
        //    }
        //}
	}

	string StaticMeshComponentEditor::Title()
	{
		return "Static Mesh";
	}
}
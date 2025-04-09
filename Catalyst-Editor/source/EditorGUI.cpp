#include "EditorGUI.h"

namespace Catalyst
{
    bool EditorGUI::ImageButtonWithLabel(ImTextureID textureID, const char* label, ImVec2 imageSize, ImVec2 buttonSize, bool requireDoubleClick)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        bool clicked = false;

        ImGui::BeginGroup();

        ImGui::PushID(label);

        // Invisible button first to define the clickable area
        if (ImGui::InvisibleButton("##button", buttonSize))
        {
            if (!requireDoubleClick)
            {
                clicked = ImGui::IsMouseClicked(0);
            }
        }

        ImU32 borderColor = ImGui::GetColorU32(ImGuiCol_Border);
        ImU32 backgroundColor = ImGui::GetColorU32(ImGuiCol_Button);

        if (ImGui::IsItemFocused())
        {
            if (requireDoubleClick)
            {
                clicked = ImGui::IsMouseDoubleClicked(0);
            }

            // **Draw background when focused**
            backgroundColor = ImGui::GetColorU32(ImGuiCol_ButtonHovered); // Or use a custom color

            // Change the border color to indicate selection (e.g., change to green)
            borderColor = ImGui::GetColorU32(ImGuiCol_ButtonActive);
        }

        // **Draw background (filled rectangle) when focused**
        ImVec2 pos = ImGui::GetItemRectMin();
        ImVec2 max = ImGui::GetItemRectMax();
        ImGui::GetWindowDrawList()->AddRectFilled(pos, max, backgroundColor, 4.0f);  // Draw background with rounded corners

        // **Draw border around the button**
        float borderThickness = 1.5f;
        ImGui::GetWindowDrawList()->AddRect(pos, max, borderColor, 4.0f, 0, borderThickness);

        // **Draw the image**
        float imageOffsetX = pos.x + (buttonSize.x - imageSize.x) * 0.5f;
        float imageOffsetY = pos.y + 5;
        ImGui::GetWindowDrawList()->AddImage(textureID,
            ImVec2(imageOffsetX, imageOffsetY),
            ImVec2(imageOffsetX + imageSize.x, imageOffsetY + imageSize.y));

        // **Draw centered label with 25% smaller font**
        string truncatedLabel = TruncateText(label, buttonSize.x - 5); // Truncate if needed
        ImVec2 textSize = ImGui::CalcTextSize(truncatedLabel.c_str());

        // Temporarily change the font size by scaling it down (25% smaller) using a custom font
        //ImGui::PushStyleVar(ImGuiStyleVar_FontSize, ImGui::GetFontSize() * 0.75f);

        // Calculate text position with scaled font
        float textOffsetX = pos.x + (buttonSize.x - textSize.x) * 0.5f;
        float textOffsetY = pos.y + imageSize.y + 10;
        ImGui::GetWindowDrawList()->AddText(ImVec2(textOffsetX, textOffsetY), ImGui::GetColorU32(ImGuiCol_Text), truncatedLabel.c_str());

        // Restore the previous font size style
        //ImGui::PopStyleVar();

        ImGui::PopID();

        ImGui::EndGroup();
        ImGui::PopStyleVar(2);

        return clicked;
    }


    string EditorGUI::TruncateText(const string& text, float maxWidth)
    {
        string truncatedText = text;
        while (ImGui::CalcTextSize(truncatedText.c_str()).x > maxWidth && truncatedText.length() > 3)
        {
            truncatedText.pop_back(); // Remove one character at a time
        }

        if (truncatedText.length() < text.length()) 
        {
            truncatedText += "..."; // Append ellipsis if truncated
        }
        return truncatedText;
    }
}
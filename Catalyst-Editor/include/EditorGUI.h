#pragma once

#include <imgui.h>

#include <string>

using std::string;

namespace Catalyst
{
	class EditorGUI
	{
	public:
		static bool ImageButtonWithLabel(ImTextureID textureID, const char* label, ImVec2 imageSize, ImVec2 buttonSize, bool requireDoubleClick = false);
		static string TruncateText(const string& text, float maxWidth);

	};
}
#include "Windows/EditorWindow.h"

#include <imgui.h>

namespace Catalyst
{
	EditorWindow::EditorWindow(string _title)
		: m_title{ std::move(_title) }, m_isOpen{ true }
	{
	}

	EditorWindow::~EditorWindow() = default;

	ImGuiWindowFlags EditorWindow::GetWindowFlags()
	{
		return ImGuiWindowFlags_NoCollapse;
	}

	void EditorWindow::Render(EditorApplication* app)
	{
		if (ImGui::Begin(m_title.c_str(), &m_isOpen, GetWindowFlags()))
		{
			OnGui(app);
		}

		ImGui::End();
	}
}

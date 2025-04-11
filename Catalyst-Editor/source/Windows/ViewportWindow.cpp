#include "Windows/ViewportWindow.h"

#include "Viewport/ViewportRenderer.h"

namespace Catalyst
{
	ViewportWindow::ViewportWindow(ViewportRenderer* renderer)
		: EditorWindow{ "Viewport" }, m_renderer{ renderer }
	{
	}

	ViewportWindow::~ViewportWindow()
	{
	}

	void ViewportWindow::OnGui(EditorApplication* app)
	{
		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		ImGui::Image((void*)(intptr_t)m_renderer->m_texture, windowSize, { 0, 1 }, { 1, 0 });
	}
}
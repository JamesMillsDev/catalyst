#pragma once

#include <string>

#include <ImGui/imgui.h>

using std::string;

namespace Catalyst
{
	class EditorWindow
	{
		friend class EditorApplication;

	public:
		EditorWindow(string _title);
		virtual ~EditorWindow();

	protected:
		string m_title;
		bool m_isOpen;

	protected:
		virtual void OnGui(EditorApplication* app) = 0;
		virtual ImGuiWindowFlags GetWindowFlags();

	private:
		void Render(EditorApplication* app);

	};
}

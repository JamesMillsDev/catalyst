#pragma once

#include <ImGui/imgui.h>

#include <ActorComponent.h>

#include <string>

using std::string;

namespace Catalyst
{
	class EditorBase
	{
		friend class InspectorWindow;

	public:
		virtual ~EditorBase() = default;

	protected:
		virtual void Render(void* _component) = 0;

	};

	template<typename T>
	class Editor : public EditorBase
	{
	protected:
		void Render(void* _component) override final;

		virtual void OnGui(T* _component) = 0;
		virtual string Title() = 0;

	};

	template<typename T>
	void Editor<T>::Render(void* _component)
	{
		static_assert(std::is_base_of_v<ActorComponent, T>, "T must derive from ActorComponent");
		
		if (T* component = dynamic_cast<T*>(static_cast<ActorComponent*>(_component)))
		{
			if (ImGui::CollapsingHeader(Title().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				OnGui(component);
			}
		}
	}
}
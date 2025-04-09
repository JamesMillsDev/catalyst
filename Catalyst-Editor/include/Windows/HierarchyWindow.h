#pragma once

#include "EditorWindow.h"

#include <Actor.h>
#include <ActorComponent.h>
#include <GameplayModule.h>

#include <unordered_map>

#include <imgui.h>

using std::unordered_map;

namespace Catalyst
{
	class GameplayModule;
	class Actor;

	class HierarchyWindow final : public EditorWindow
	{
		friend class InspectorWindow;

	public:
		// Struct to hold the actor info (name, icon, etc.)
		struct ActorInfo
		{
		public:
			Actor* actor;
			string name;
			bool isEditingName = false;
			bool hideFromHierarchy = false;
			bool enabled = true;

		public:
			ActorInfo();
			ActorInfo(Actor* actor);

		};

	public:
		HierarchyWindow();
		~HierarchyWindow() override;

	public:
		Actor* GetSelectedActor();

	protected:
		void OnGui(EditorApplication* app) override;
		ImGuiWindowFlags GetWindowFlags() override;

	private:
		GameplayModule* m_module;

		Actor* m_selectedActor;
		Actor* m_editingActor;

		unordered_map<Actor*, ActorInfo> m_actorInfoMap;
		bool m_hasSelectedThisFrame;
		bool m_isDragging;

	private:
		void DisplayActor(Actor* _actor);
		void DisplayActorNode(Actor* _actor, ImGuiTreeNodeFlags& flags, bool isSelected, bool& isOpen, float fullWidth, ImVec2 selectableSize);

		void EditActorName(Actor* _actor, ActorInfo& _actorInfo, float _fullWidth, float _indentLevel);
		ActorInfo& GetActorInfo(Actor* _actor);

		ImGuiTreeNodeFlags GetBaseFlags(bool hasChildren);
		void HandleDragDropReparent(Actor* _actor);
		void HandleDeselection();

		template <derived<ActorComponent> COMPONENT>
		COMPONENT* CreateActorWith(string _name);

	};

	template<derived<ActorComponent> COMPONENT>
	COMPONENT* HierarchyWindow::CreateActorWith(string _name)
	{
		Actor* actor = m_module->SpawnActor<Actor>();
		actor->SetName(_name);

		return actor->CreateComponent<COMPONENT>();
	}
}
#pragma once

#include "EditorWindow.h"

#include <list>

using std::list;

namespace Catalyst
{
	class Actor;
	class ActorTransform;

	class HierarchyWindow;
	class EditorBase;

	class Quaternion;
	class Vector3;

	class InspectorWindow : public EditorWindow
	{
	public:
		InspectorWindow(HierarchyWindow* _hierarchy);
		~InspectorWindow() override;

	protected:
		void OnGui(EditorApplication* app) override;

	private:
		HierarchyWindow* m_hierarchy;

		list<EditorBase*> m_guis;

	private:
		void DoTransformInspector(ActorTransform* _transform);

		bool DoVector3Slider(const string& _label, Vector3* _vector);
		bool DoRotationSlider(const string& _label, Quaternion* _quat);

	};
}
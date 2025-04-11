#pragma once

#include "EditorWindow.h"

namespace Catalyst
{
	class ViewportRenderer;

	class ViewportWindow : public EditorWindow
	{
	public:
		ViewportWindow(ViewportRenderer* renderer);
		~ViewportWindow() override;

	protected:
		void OnGui(EditorApplication* app) override;

	private:
		ViewportRenderer* m_renderer;

	};
}
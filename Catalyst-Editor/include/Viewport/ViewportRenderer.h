#pragma once

namespace Catalyst
{
	class ViewportRenderer
	{
		friend class EditorApplication;
		friend class ViewportWindow;

	private:
		unsigned int m_fbo;
		unsigned int m_texture;
		unsigned int m_rbo;

	private:
		ViewportRenderer(int width, int height);
		~ViewportRenderer();

	private:
		void Resize(int width, int height);
		void Bind();
		void Unbind();

	};
}
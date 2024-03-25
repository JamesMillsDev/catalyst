#pragma once

#include <Catalyst/Catalyst.hpp>
#include <Catalyst/Engine/Utility/Color.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

using std::string;

namespace Catalyst
{
	class Screen
	{
	public:
		/* Gets the current width of the screen. */
		[[nodiscard]] DLL const int& Width() const;
		/* Gets the current height of the screen. */
		[[nodiscard]] DLL const int& Height() const;
		/* Gets the current title of the window. */
		DLL const string& Title();
		/* Gets the current clear colour of the screen. */
		[[nodiscard]] DLL const Color& ClearColor() const;

		/* Access to the GLFW window. */
		[[nodiscard]] DLL GLFWwindow* GetWindowPtr() const;

		/*
		* Sets the width of the screen to the new value.
		* @param _newWidth: The new width of the screen.
		*/
		DLL void SetWidth(int _newWidth);
		/*
		* Sets the height of the screen to the new value.
		* @param _newHeight: The new height of the screen.
		*/
		DLL void SetHeight(int _newHeight);
		/*
		* Sets the title of the window to the new value.
		* @param _newTitle: The new title of the window shown in the title bar.
		*/
		DLL void SetTitle(const char* _newTitle);
		/*
		* Sets the clear color of the screen to the new value.
		* @param _newColor: The new background colour of the screen.
		*/
		DLL void SetClearColor(Color _newColor);

		[[nodiscard]] DLL bool WindowShouldClose() const;
		DLL void CloseWindow();

	private:
		friend class BaseApplication;

	private:
		GLFWwindow* m_window;
		bool m_closeWindow;
		bool m_vSync;

		int m_width;
		int m_height;
		string m_title;
		Color m_clearColor;

	private:
		DLL Screen();
		DLL ~Screen();

	private:
		DLL int Open(const shared_ptr<class Config>& _config);
		DLL void Close();

		DLL bool BeginFrame();
		DLL void EndFrame();

		DLL GLFWwindow*& Context();

		DLL int LoadValuesFrom(const shared_ptr<class Config>& _config);

	};
}
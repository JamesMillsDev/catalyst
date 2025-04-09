//  ***************************************************************
//  Screen - Creation date: 6/9/2024 5:13:30 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"
#include "Mathematics/Color.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <memory>

using std::string;
using std::shared_ptr;

namespace Catalyst
{
	class Config;

	class DLL Screen
	{
	public:
		/* Gets the current width of the screen. */
		[[nodiscard]] const int& Width() const;
		/* Gets the current height of the screen. */
		[[nodiscard]] const int& Height() const;
		/* Gets the current title of the window. */
		const string& Title();
		/* Gets the current clear colour of the screen. */
		[[nodiscard]] const Color& ClearColor() const;

		/* Access to the GLFW window. */
		[[nodiscard]] GLFWwindow* GetWindowPtr() const;

		/*
		* Sets the width of the screen to the new value.
		* @param _newWidth: The new width of the screen.
		*/
		void SetWidth(int _newWidth);
		/*
		* Sets the height of the screen to the new value.
		* @param _newHeight: The new height of the screen.
		*/
		void SetHeight(int _newHeight);
		/*
		* Sets the title of the window to the new value.
		* @param _newTitle: The new title of the window shown in the title bar.
		*/
		void SetTitle(const char* _newTitle);
		/*
		* Sets the clear color of the screen to the new value.
		* @param _newColor: The new background colour of the screen.
		*/
		void SetClearColor(Color _newColor);

		[[nodiscard]] bool WindowShouldClose() const;
		void CloseWindow();

	private:
		friend class Application;

	private:
		GLFWwindow* m_window;
		bool m_closeWindow;
		bool m_vSync;

		int m_width;
		int m_height;
		string m_title;
		Color m_clearColor;

	private:
		Screen();
		~Screen();

	private:
		int Open(Config* _config);
		void Close() const;

		bool BeginFrame();
		void EndFrame() const;

		GLFWwindow*& Context();

		int LoadValuesFrom(Config* _config);

	};
}
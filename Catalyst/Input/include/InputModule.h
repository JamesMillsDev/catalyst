//  ***************************************************************
//  InputModule - Creation date: 6/8/2024 2:35:47 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "IModule.h"

#include "Actions/InputAction.h"

#include <vector>
#include <map>

#include <glm/vec2.hpp>

using std::vector;
using std::map;

using glm::vec2;

// ReSharper disable once CppInconsistentNaming
struct GLFWwindow;

namespace Catalyst
{
	class DLL InputModule final : public IModule
	{
		// delegates for attaching input observers to the Input class
		typedef void(*KeyCallback)(GLFWwindow*, int, int, int, int);
		typedef void(*MouseButtonCallback)(GLFWwindow*, int, int, int);
		typedef void(*MouseScrollCallback)(GLFWwindow*, double, double);
		typedef void(*MouseMoveCallback)(GLFWwindow*, double, double);

		typedef void(*ListChange)(const char*, Action*);

		friend class Application;

	public:
		InputModule(const InputModule&) = delete;
		InputModule(InputModule&&) noexcept = delete;

	public:
		template<typename ACTION>
		static ACTION* Find(const char* _id);
		static bool AddAction(const char* _id, Action* _action);

		static vec2 GetMousePosition();
		static vec2 GetPrevMousePosition();

		// query the keyboard state
		static bool IsButtonDown(Binding _binding);
		static bool IsButtonUp(Binding _binding);

		// returns true if the key was pressed / released this frame
		static bool WasButtonPressed(Binding _binding);
		static bool WasButtonReleased(Binding _binding);

	public:
		InputModule& operator=(const InputModule&) = delete;
		InputModule& operator=(InputModule&&) noexcept = delete;

	private:
		friend void MakeInputModuleInstance();

	private:
		static InputModule* m_instance;

	private:
		map<const char*, Action*> m_actions;

		list<int> m_pressedKeys;

		float m_mouseX;
		float m_mouseY;
		float m_oldMouseX;
		float m_oldMouseY;
		double m_scroll;

		bool m_firstMouseMove;

		int* m_lastKeys;
		int* m_currentKeys;
		int* m_lastButtons;
		int* m_currentButtons;

		list<KeyCallback> m_keyCallbacks;
		list<MouseMoveCallback>	m_mouseMoveCallbacks;
		list<MouseButtonCallback> m_mouseButtonCallbacks;
		list<MouseScrollCallback> m_mouseScrollCallbacks;

	private:
		InputModule();
		~InputModule() override;

	private:
		// only want the Application class to be able to create / destroy
		static InputModule* GetInstance();

		void Enter() override;
		void Exit() override;
		void Tick() override;

		void OnMouseMove(float _newXPos, float _newYPos);

	};

	template <typename ACTION>
	ACTION* InputModule::Find(const char* _id)
	{
		if (GetInstance()->m_actions.contains(_id))
			return dynamic_cast<ACTION*>(GetInstance()->m_actions[_id]);

		return nullptr;
	}
}

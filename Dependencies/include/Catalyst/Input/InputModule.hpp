#pragma once

#include <Catalyst/Catalyst.hpp>
#include <Catalyst/Engine/IModule.hpp>
#include <catalyst/Input/InputAction.hpp>
#include <catalyst/Input/Bindings.hpp>

#include <vector>
#include <map>

#include <glm/vec2.hpp>

using std::vector;
using std::map;

using glm::vec2;

struct GLFWwindow;

namespace Catalyst
{
	class InputModule final : public IModule
	{
		// delegates for attaching input observers to the Input class
		typedef void(*KeyCallback)(GLFWwindow*, int, int, int, int);
		typedef void(*MouseButtonCallback)(GLFWwindow*, int, int, int);
		typedef void(*MouseScrollCallback)(GLFWwindow*, double, double);
		typedef void(*MouseMoveCallback)(GLFWwindow*, double, double);

		typedef void(*ListChange)(const char*, Action*);

		friend class Application;

	public:
		template<typename ACTION>
		static ACTION* Find(const char* _id);
		DLL static bool AddAction(const char* _id, Action* _action);

		DLL static vec2 GetMousePosition();
		DLL static vec2 GetPrevMousePosition();

		// query the keyboard state
		DLL static bool IsButtonDown(Binding _binding);
		DLL static bool IsButtonUp(Binding _binding);

		// returns true if the key was pressed / released this frame
		DLL static bool WasButtonPressed(Binding _binding);
		DLL static bool WasButtonReleased(Binding _binding);

	private:
		DLL static InputModule* m_instance;

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
		DLL InputModule();
		DLL ~InputModule();

	private:
		// only want the Application class to be able to create / destroy
		DLL static InputModule* GetInstance();

		DLL void Tick(class Application* _app) override;
		DLL void OnMouseMove(float _newXPos, float _newYPos);

	};

	template <typename ACTION>
	ACTION* InputModule::Find(const char* _id)
	{
		if (GetInstance()->m_actions.contains(_id))
			return dynamic_cast<ACTION*>(GetInstance()->m_actions[_id]);

		return nullptr;
	}
}

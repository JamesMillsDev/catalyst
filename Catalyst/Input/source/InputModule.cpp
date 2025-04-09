// ReSharper disable CppTooWideScope
#include "cainputpch.h"
#include "InputModule.h"

#include <iostream>
#include <ranges>
#include <GLFW/glfw3.h>

namespace Catalyst
{
	InputModule* InputModule::m_instance = nullptr;

	bool InputModule::AddAction(const char* _id, Action* _action)
	{
		if (m_instance->m_actions.contains(_id))
			return false;

		m_instance->m_actions[_id] = _action;
		return true;
	}

	vec2 InputModule::GetMousePosition()
	{
		return
		{
			m_instance->m_mouseX,
			m_instance->m_mouseY
		};
	}

	vec2 InputModule::GetPrevMousePosition()
	{
		return
		{
			m_instance->m_oldMouseX,
			m_instance->m_oldMouseY
		};
	}

	bool InputModule::IsButtonDown(const Binding _binding)
	{
		const bool keyPress = m_instance->m_currentKeys[_binding] == GLFW_PRESS;
		if (keyPress)
			return true;

		const bool mousePress = m_instance->m_currentButtons[_binding] == GLFW_PRESS;
		return mousePress;
	}

	bool InputModule::IsButtonUp(const Binding _binding)
	{
		const bool keyRelease = m_instance->m_currentKeys[_binding] == GLFW_RELEASE;
		if (keyRelease)
			return true;

		const bool mouseRelease = m_instance->m_currentButtons[_binding] == GLFW_RELEASE;
		return mouseRelease;
	}

	bool InputModule::WasButtonPressed(const Binding _binding)
	{
		const bool wasKeyPressed = m_instance->m_currentKeys[_binding] == GLFW_PRESS &&
			m_instance->m_lastKeys[_binding] == GLFW_RELEASE;

		if (wasKeyPressed)
			return true;

		const bool wasMousePressed = m_instance->m_currentButtons[_binding] == GLFW_PRESS &&
			m_instance->m_lastButtons[_binding] == GLFW_RELEASE;

		if (wasMousePressed)
			return true;

		return false;
	}

	bool InputModule::WasButtonReleased(const Binding _binding)
	{
		const bool wasKeyReleased = m_instance->m_currentKeys[_binding] == GLFW_RELEASE &&
			m_instance->m_lastKeys[_binding] == GLFW_PRESS;

		if (wasKeyReleased)
			return true;

		const bool wasMouseReleased = m_instance->m_currentButtons[_binding] == GLFW_RELEASE &&
			m_instance->m_lastButtons[_binding] == GLFW_PRESS;

		if (wasMouseReleased)
			return true;

		return false;
	}

	float InputModule::GetMouseScroll()
	{
		return static_cast<float>(m_instance->m_scroll);
	}

	InputModule::InputModule()
		: m_mouseX{ 0 }, m_mouseY{ 0 }, m_oldMouseX{ 0 }, m_oldMouseY{ 0 }, m_scroll{ 0 },
		m_firstMouseMove{ false }, m_lastKeys{ new int[GLFW_KEY_LAST + 1] },
		m_currentKeys{ new int[GLFW_KEY_LAST + 1] }, m_lastButtons{ new int[GLFW_MOUSE_BUTTON_LAST + 1] },
		m_currentButtons{ new int[GLFW_MOUSE_BUTTON_LAST + 1] }
	{
		m_instance = this;

		AddModule(this);
	}

	InputModule::~InputModule()
	{
		delete[] m_lastKeys;
		delete[] m_currentKeys;

		for (const auto actions : m_actions | std::views::values)
			delete actions;
	}

	InputModule* InputModule::GetInstance()
	{
		return m_instance;
	}

	void InputModule::BeginPlay()
	{
		GLFWwindow* window = glfwGetCurrentContext();

		for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; ++i)
			m_lastKeys[i] = m_currentKeys[i] = glfwGetKey(window, i);

		for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; ++i)
			m_lastButtons[i] = m_currentButtons[i] = glfwGetMouseButton(window, i);

		// set up callbacks
		auto keyPressCallback = [](GLFWwindow* _window, const int _key, const int _scanCode, const int _action, const int _mods)
			{
				for (const auto& f : m_instance->m_keyCallbacks)
					f(_window, _key, _scanCode, _action, _mods);
			};

		auto mouseMoveCallback = [](GLFWwindow* _window, const double _x, const double _y)
			{
				int w = 0, h = 0;
				glfwGetWindowSize(_window, &w, &h);

				m_instance->OnMouseMove(static_cast<float>(_x), static_cast<float>(h) - static_cast<float>(_y));

				for (const auto& f : m_instance->m_mouseMoveCallbacks)
					f(_window, _x, h - _y);
			};

		auto mouseInputCallback = [](GLFWwindow* _window, const int _button, const int _action, const int _mods)
			{
				for (const auto& f : m_instance->m_mouseButtonCallbacks)
					f(_window, _button, _action, _mods);
			};

		auto mouseScrollCallback = [](GLFWwindow* _window, const double _xOffset, const double _yOffset)
			{
				m_instance->m_scroll += _yOffset;

				for (const auto& f : m_instance->m_mouseScrollCallbacks)
					f(_window, _xOffset, _yOffset);
			};

		auto mouseEnterCallback = [](GLFWwindow*, int)
			{
				// Set flag to prevent large mouse delta on entering screen
				m_instance->m_firstMouseMove = true;
			};

		glfwSetKeyCallback(window, keyPressCallback);
		glfwSetMouseButtonCallback(window, mouseInputCallback);
		glfwSetCursorPosCallback(window, mouseMoveCallback);
		glfwSetScrollCallback(window, mouseScrollCallback);
		glfwSetCursorEnterCallback(window, mouseEnterCallback);
	}

	void InputModule::Enter() { }

	void InputModule::Exit() { }

	void InputModule::Tick()
	{
		GLFWwindow* window = glfwGetCurrentContext();

		m_pressedKeys.clear();

		// Update keys
		for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; ++i)
		{
			m_lastKeys[i] = m_currentKeys[i];

			if ((m_currentKeys[i] = glfwGetKey(window, i)) == GLFW_PRESS)
				m_pressedKeys.push_back(m_currentKeys[i]);
		}

		// Update mouse
		for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; ++i)
		{
			m_lastButtons[i] = m_currentButtons[i];
			m_currentButtons[i] = glfwGetMouseButton(window, i);
		}

		for (const auto action : m_actions | std::views::values)
			action->Tick();

		// Update old mouse position
		m_oldMouseX = m_mouseX;
		m_oldMouseY = m_mouseY;

		m_scroll = 0;
	}

	void InputModule::OnMouseMove(const float _newXPos, const float _newYPos)
	{
		m_mouseX = _newXPos;
		m_mouseY = _newYPos;

		if (m_firstMouseMove)
		{
			// On first move after Startup/entering window reset old mouse position
			m_oldMouseX = _newXPos;
			m_oldMouseY = _newYPos;
			m_firstMouseMove = false;
		}
	}
}

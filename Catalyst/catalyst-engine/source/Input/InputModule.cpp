#include <Catalyst/Input/InputModule.hpp>

#include <GLFW/glfw3.h>

namespace Catalyst
{
	InputModule* InputModule::m_instance = nullptr;

	InputModule::InputModule()
		: IModule("Input"), m_mouseX{ 0 }, m_mouseY{ 0 }, m_oldMouseX{ 0 },
		m_oldMouseY{ 0 }, m_mouseScroll{ 0 }, m_firstMouseMove{ false }
	{
		m_instance = this;

		// track current/previous key and mouse button states
		m_lastKeys = new int[GLFW_KEY_LAST + 1];
		m_currentKeys = new int[GLFW_KEY_LAST + 1];

		const auto window = glfwGetCurrentContext();

		for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; ++i)
			m_lastKeys[i] = m_currentKeys[i] = glfwGetKey(window, i);

		for (int i = 0; i < 8; ++i)
			m_lastButtons[i] = m_currentButtons[i] = glfwGetMouseButton(window, i);

		// set up callbacks
		auto keyPressCallback = [](GLFWwindow* _window, const int _key, const int _scanCode, const int _action, const int _mods)
			{
				for (auto& f : GetInstance()->m_keyCallbacks)
					f(_window, _key, _scanCode, _action, _mods);
			};

		auto characterInputCallback = [](GLFWwindow* _window, const unsigned int _character)
			{
				GetInstance()->m_pressedCharacters.push_back(_character);

				for (auto& f : GetInstance()->m_charCallbacks)
					f(_window, _character);
			};

		auto mouseMoveCallback = [](GLFWwindow* _window, const double _x, const double _y)
			{
				int w = 0, h = 0;
				glfwGetWindowSize(_window, &w, &h);

				GetInstance()->OnMouseMove(static_cast<int>(_x), h - static_cast<int>(_y));

				for (auto& f : InputModule::GetInstance()->m_mouseMoveCallbacks)
					f(_window, _x, h - _y);
			};

		auto mouseInputCallback = [](GLFWwindow* _window, const int _button, const int _action, const int _mods)
			{
				for (auto& f : GetInstance()->m_mouseButtonCallbacks)
					f(_window, _button, _action, _mods);
			};

		auto mouseScrollCallback = [](GLFWwindow* _window, const double _xOffset, const double _yOffset)
			{
				GetInstance()->m_mouseScroll += _yOffset;

				for (auto& f : GetInstance()->m_mouseScrollCallbacks)
					f(_window, _xOffset, _yOffset);
			};

		auto mouseEnterCallback = [](GLFWwindow* _window, int _entered)
			{
				// Set flag to prevent large mouse delta on entering screen
				GetInstance()->m_firstMouseMove = true;
			};

		glfwSetKeyCallback(window, keyPressCallback);
		glfwSetCharCallback(window, characterInputCallback);
		glfwSetMouseButtonCallback(window, mouseInputCallback);
		glfwSetCursorPosCallback(window, mouseMoveCallback);
		glfwSetScrollCallback(window, mouseScrollCallback);
		glfwSetCursorEnterCallback(window, mouseEnterCallback);
	}

	InputModule::~InputModule()
	{
		IModule::~IModule();

		delete[] m_lastKeys;
		delete[] m_currentKeys;
	}

	bool InputModule::IsKeyDown(const int _inputKeyId) const
	{
		return m_currentKeys[_inputKeyId] == GLFW_PRESS;
	}

	bool InputModule::IsKeyUp(const int _inputKeyId) const
	{
		return m_currentKeys[_inputKeyId] == GLFW_RELEASE;
	}

	bool InputModule::WasKeyPressed(const int _inputKeyId) const
	{
		return m_currentKeys[_inputKeyId] == GLFW_PRESS && m_lastKeys[_inputKeyId] == GLFW_RELEASE;
	}

	bool InputModule::WasKeyReleased(const int _inputKeyId) const
	{
		return m_currentKeys[_inputKeyId] == GLFW_RELEASE && m_lastKeys[_inputKeyId] == GLFW_PRESS;
	}

	const vector<int>& InputModule::GetPressedKeys() const
	{
		return m_pressedKeys;
	}

	const vector<unsigned>& InputModule::GetPressedCharacters() const
	{
		return m_pressedCharacters;
	}

	bool InputModule::IsMouseButtonDown(const int _inputMouseId) const
	{
		return m_currentButtons[_inputMouseId] == GLFW_PRESS;
	}

	bool InputModule::IsMouseButtonUp(const int _inputMouseId) const
	{
		return m_currentButtons[_inputMouseId] == GLFW_RELEASE;
	}

	bool InputModule::WasMouseButtonPressed(const int _inputMouseId) const
	{
		return m_currentButtons[_inputMouseId] == GLFW_PRESS && m_lastButtons[_inputMouseId] == GLFW_RELEASE;
	}

	bool InputModule::WasMouseButtonReleased(const int _inputMouseId) const
	{
		return m_currentButtons[_inputMouseId] == GLFW_RELEASE && m_lastButtons[_inputMouseId] == GLFW_PRESS;
	}

	int InputModule::GetMouseX() const
	{
		return m_mouseX;
	}

	int InputModule::GetMouseY() const
	{
		return m_mouseY;
	}

	void InputModule::GetMousePos(int* _x, int* _y) const
	{
		if (_x != nullptr) 
			*_x = m_mouseX;

		if (_y != nullptr) 
			*_y = m_mouseY;
	}

	int InputModule::GetMouseDeltaX() const
	{
		return m_mouseX - m_oldMouseX;
	}

	int InputModule::GetMouseDeltaY() const
	{
		return m_mouseY - m_oldMouseY;
	}

	void InputModule::GetMouseDelta(int* _x, int* _y) const
	{
		if (_x != nullptr) 
			*_x = m_mouseX - m_oldMouseX;

		if (_y != nullptr) 
			*_y = m_mouseY - m_oldMouseY;
	}

	double InputModule::GetMouseScroll() const
	{
		return m_mouseScroll;
	}

	void InputModule::Tick(Application* _app)
	{
		m_pressedCharacters.clear();

		const auto window = glfwGetCurrentContext();

		m_pressedKeys.clear();

		// Update keys
		for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; ++i)
		{
			m_lastKeys[i] = m_currentKeys[i];

			if ((m_currentKeys[i] = glfwGetKey(window, i)) == GLFW_PRESS)
				m_pressedKeys.push_back(m_currentKeys[i]);
		}

		// Update mouse
		for (int i = 0; i < 8; ++i)
		{
			m_lastButtons[i] = m_currentButtons[i];
			m_currentButtons[i] = glfwGetMouseButton(window, i);
		}

		// Update old mouse position
		m_oldMouseX = m_mouseX;
		m_oldMouseY = m_mouseY;
	}

	void InputModule::OnMouseMove(const int _newXPos, const int _newYPos)
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

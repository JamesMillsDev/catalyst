#pragma once

#include <Catalyst/Catalyst.hpp>
#include <Catalyst/Engine/IModule.hpp>

#include <vector>
#include <functional>
#include <map>

using std::vector;
using std::function;
using std::map;

struct GLFWwindow;

namespace Catalyst
{
	// a giant list of input codes for keyboard and mouse
	enum EInputCodes : int
	{
		Unknown = -1,
		Space = 32,
		Apostrophe = 39,
		Comma = 44,
		Minus = 45,
		Period = 46,
		Slash = 47,
		Key0 = 48,
		Key1 = 49,
		Key2 = 50,
		Key3 = 51,
		Key4 = 52,
		Key5 = 53,
		Key6 = 54,
		Key7 = 55,
		Key8 = 56,
		Key9 = 57,
		Semicolon = 59,
		Equal = 61,
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
		LeftBracket = 91,
		Backslash = 92,
		RightBracket = 93,
		Grave = 96,
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		RightArrow = 262,
		LeftArrow = 263,
		DownArrow = 264,
		UpArrow = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,
		Keypad0 = 320,
		Keypad1 = 321,
		Keypad2 = 322,
		Keypad3 = 323,
		Keypad4 = 324,
		Keypad5 = 325,
		Keypad6 = 326,
		Keypad7 = 327,
		Keypad8 = 328,
		Keypad9 = 329,
		KeypadDecimal = 330,
		KeypadDivide = 331,
		KeypadMultiply = 332,
		KeypadSubtract = 333,
		KeypadAdd = 334,
		KeypadEnter = 335,
		KeypadEqual = 336,
		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		LeftMouseButton = 0,
		RightMouseButton = 1,
		MiddleMouseButton = 2,
		MouseButton4 = 3,
		MouseButton5 = 4,
		MouseButton6 = 5,
		MouseButton7 = 6,
		MouseButton8 = 7,
	};

	class InputModule final : public IModule
	{
		friend class Application;
	public:
		// returns access to the singleton instance
		static InputModule* GetInstance() { return m_instance; }

	public:
		// query the keyboard state
		DLL bool IsKeyDown(int _inputKeyId) const;
		DLL bool IsKeyUp(int _inputKeyId) const;

		// returns true if the key was pressed / released this frame
		DLL bool WasKeyPressed(int _inputKeyId) const;
		DLL bool WasKeyReleased(int _inputKeyId) const;

		// returns access to all keys that are currently pressed
		DLL const vector<int>& GetPressedKeys() const;
		DLL const vector<unsigned int>& GetPressedCharacters() const;

		// query the mouse button state
		DLL bool IsMouseButtonDown(int _inputMouseId) const;
		DLL bool IsMouseButtonUp(int _inputMouseId) const;

		// returns true if the button was pressed / released this frame
		DLL bool WasMouseButtonPressed(int _inputMouseId) const;
		DLL bool WasMouseButtonReleased(int _inputMouseId) const;

		// query the mouse position
		DLL int GetMouseX() const;
		DLL int GetMouseY() const;
		DLL void GetMousePos(int* _x, int* _y) const;

		// query mouse movement
		DLL int GetMouseDeltaX() const;
		DLL int GetMouseDeltaY() const;
		DLL void GetMouseDelta(int* _x, int* _y) const;

		// query how far the mouse wheel has been moved 
		DLL double GetMouseScroll() const;

		// delgates for attaching input observers to the Input class
		typedef function<void(GLFWwindow* _window, int _key, int _scanCode, int _action, int _mods)> KeyCallback;
		typedef function<void(GLFWwindow* _window, unsigned int _character)> CharCallback;
		typedef function<void(GLFWwindow* _window, int _button, int _action, int _mods)> MouseButtonCallback;
		typedef function<void(GLFWwindow* _window, double _xOffset, double _yOffset)> MouseScrollCallback;
		typedef function<void(GLFWwindow* _window, double _x, double _y)> MouseMoveCallback;

		// functions for attatching input observers
		DLL void AttachKeyObserver(const KeyCallback& _callback) { m_keyCallbacks.push_back(_callback); }
		DLL void AttachCharObserver(const CharCallback& _callback) { m_charCallbacks.push_back(_callback); }
		DLL void AttachMouseButtonObserver(const MouseButtonCallback& _callback) { m_mouseButtonCallbacks.push_back(_callback); }
		DLL void AttachMouseMoveObserver(const MouseMoveCallback& _callback) { m_mouseMoveCallbacks.push_back(_callback); }
		DLL void AttachMouseScrollObserver(const MouseScrollCallback& _callback) { m_mouseScrollCallbacks.push_back(_callback); }

	protected:
		// singleton pointer
		DLL static InputModule* m_instance;

	protected:
		// should be called once by the application each frame after the current update
		// or before glfwPollEvents
		DLL void Tick(class Application* _app) override;

	private:
		DLL InputModule();
		DLL ~InputModule() override;

	private:
		vector<int>	m_pressedKeys;
		vector<unsigned int> m_pressedCharacters;

		int	m_mouseX;
		int	m_mouseY;
		int	m_oldMouseX;
		int	m_oldMouseY;
		double m_mouseScroll;

		bool m_firstMouseMove;	// flag for first mouse input after start or mouse entering window

		DLL void OnMouseMove(int _newXPos, int _newYPos);

		vector<KeyCallback>	m_keyCallbacks;
		vector<CharCallback> m_charCallbacks;
		vector<MouseMoveCallback> m_mouseMoveCallbacks;
		vector<MouseButtonCallback> m_mouseButtonCallbacks;
		vector<MouseScrollCallback> m_mouseScrollCallbacks;

		// used to track down/up/released/pressed
		int* m_lastKeys, * m_currentKeys;
		int m_lastButtons[8], m_currentButtons[8];

	};
}

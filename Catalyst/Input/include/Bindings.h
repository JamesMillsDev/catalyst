//  ***************************************************************
//  Bindings - Creation date: 6/18/2024 11:08:36 AM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"
#include <cstdint>

namespace Catalyst
{
	typedef int32_t Binding;

	enum class EKey : Binding
	{
		UnKnown = -1,
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
		NumPad0 = 320,
		NumPad1 = 321,
		NumPad2 = 322,
		NumPad3 = 323,
		NumPad4 = 324,
		NumPad5 = 325,
		NumPad6 = 326,
		NumPad7 = 327,
		NumPad8 = 328,
		NumPad9 = 329,
		NumPadDecimal = 330,
		NumPadDivide = 331,
		NumPadMultiply = 332,
		NumPadSubtract = 333,
		NumPadAdd = 334,
		NumPadEnter = 335,
		NumPadEqual = 336,
		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346
	};

	DEFINE_ENUM_FLAG_OPERATORS(EKey);

	enum class EMouseButton : Binding
	{
		UnKnown = -1,
		Left = 0,
		Right = 1,
		Middle = 2,
		Button4 = 3,
		Button5 = 4,
		Button6 = 5,
		Button7 = 6,
		Button8 = 7,
	};

	DEFINE_ENUM_FLAG_OPERATORS(EMouseButton);

	enum class EAxis : Binding
	{
		UnKnown = -1,
		MousePos = 0,
		MouseDelta = 1
	};
}
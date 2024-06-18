//  ***************************************************************
//  InputAction2DAxis - Creation date: 6/18/2024 11:15:21 AM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "InputAction.h"
#include <glm/vec2.hpp>

using glm::vec2;

namespace Catalyst
{
	class DLL InputAction2DAxis : public InputAction<vec2>
	{
	public:
		explicit InputAction2DAxis(Binding _binding);

	protected:
		void UpdateValue(float _x, float _y) override;
		void Tick() override;

	};
}

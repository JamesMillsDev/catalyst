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

#include "Mathematics/Vector2.h"

namespace Catalyst
{
	class DLL InputAction2DAxis : public InputAction<Vector2>
	{
	public:
		explicit InputAction2DAxis(Binding _binding);

	protected:
		void UpdateValue(float _x, float _y) override;
		void Tick() override;

	};
}

//  ***************************************************************
//  InputAction1DAxis - Creation date: 6/18/2024 11:16:00 AM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "InputAction.h"

namespace Catalyst
{
	class DLL InputAction1DAxis : public InputAction<float>
	{
	public:
		explicit InputAction1DAxis(Binding _binding);

	protected:
		void UpdateValue(float _value) override;
		void Tick() override;

	};
}

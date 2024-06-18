//  ***************************************************************
//  InputActionButton - Creation date: 6/18/2024 11:14:39 AM
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
	class DLL InputActionButton final : public InputAction<bool>
	{
	public:
		explicit InputActionButton(Binding _binding);

	protected:
		void UpdateValue(bool _b) override;
		void Tick() override;

	};
}

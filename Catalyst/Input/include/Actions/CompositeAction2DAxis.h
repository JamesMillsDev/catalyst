//  ***************************************************************
//  CompositeAction2DAxis - Creation date: 6/18/2024 11:16:58 AM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "InputAction2DAxis.h"

namespace Catalyst
{
	class DLL CompositeAction2DAxis final : public InputAction2DAxis
	{
	public:
		CompositeAction2DAxis(EKey _xPos, EKey _xNeg, EKey _yPos, EKey _yNeg);

	protected:
		void Tick() override;

	private:
		Binding m_bindings[4];

	};
}

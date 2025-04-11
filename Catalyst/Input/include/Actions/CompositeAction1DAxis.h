//  ***************************************************************
//  CompositeAction1DAxis - Creation date: 6/18/2024 11:16:33 AM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once
#include "InputAction1DAxis.h"

namespace Catalyst
{
	class DLL CompositeAction1DAxis final : public InputAction1DAxis
	{
	public:
		CompositeAction1DAxis(EKey _pos, EKey _neg);

	protected:
		void Tick() override;

	private:
		Binding m_bindings[2];

	};
}

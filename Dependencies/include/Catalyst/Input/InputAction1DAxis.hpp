#pragma once

#include <Catalyst/Catalyst.hpp>
#include <Catalyst/Input/InputAction.hpp>

namespace Catalyst
{
	class InputAction1DAxis : public InputAction<float>
	{
	public:
		DLL InputAction1DAxis(Binding _binding);

	protected:
		DLL void UpdateValue(float _value) override;

	};
}
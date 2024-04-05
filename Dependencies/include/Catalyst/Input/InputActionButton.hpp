#pragma once

#include <Catalyst/Catalyst.hpp>
#include <Catalyst/Input/InputAction.hpp>

namespace Catalyst
{
	class InputActionButton final : public InputAction<bool>
	{
	public:
		DLL InputActionButton(Binding _binding);

	protected:
		DLL void UpdateValue(bool _b) override;
		DLL void Tick() override;

	};
}
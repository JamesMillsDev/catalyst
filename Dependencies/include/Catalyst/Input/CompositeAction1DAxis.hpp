#pragma once

#include <Catalyst/Catalyst.hpp>
#include <Catalyst/Input/InputAction1DAxis.hpp>

namespace Catalyst
{
	class CompositeAction1DAxis final : public InputAction1DAxis
	{
	public:
		DLL CompositeAction1DAxis(EKey _pos, EKey _neg);

	protected:
		DLL void Tick() override;

	private:
		Binding m_bindings[2];

	};

}
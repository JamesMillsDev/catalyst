#pragma once

#include <Catalyst/Catalyst.hpp>
#include <Catalyst/Input/InputAction2DAxis.hpp>

#include <glm/vec2.hpp>

using glm::vec2;

namespace Catalyst
{
	class CompositeAction2DAxis final : public InputAction2DAxis
	{
	public:
		DLL CompositeAction2DAxis(EKey _xPos, EKey _xNeg, EKey _yPos, EKey _yNeg);

	protected:
		DLL void Tick() override;

	private:
		Binding m_bindings[4];

	};
}
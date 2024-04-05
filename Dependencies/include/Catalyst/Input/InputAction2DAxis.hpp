#pragma once

#include <Catalyst/Catalyst.hpp>
#include <Catalyst/Input/InputAction.hpp>
#include <glm/vec2.hpp>

using glm::vec2;

namespace Catalyst
{
	class InputAction2DAxis : public InputAction<vec2>
	{
	public:
		DLL InputAction2DAxis(Binding _binding);

	protected:
		DLL void UpdateValue(float _x, float _y) override;
		DLL void Tick() override;

	};
}
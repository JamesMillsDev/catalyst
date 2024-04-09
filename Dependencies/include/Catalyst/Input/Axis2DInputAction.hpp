#pragma once

#include <Catalyst/Input/InputModule.hpp>
#include <Catalyst/Input/InputAction.hpp>

#include <glm/vec2.hpp>

using glm::vec2;

namespace Catalyst
{
	class Axis2DInputAction final : public InputAction<vec2>
	{
	public:
		Axis2DInputAction(EAxisType _axis);

	public:
		void OnTick() override;

	private:
		EAxisType m_axis;

	};
}

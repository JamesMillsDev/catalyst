#include "cainputpch.h"
#include "Actions/CompositeAction2DAxis.h"

#include "InputModule.h"

namespace Catalyst
{
	CompositeAction2DAxis::CompositeAction2DAxis(const EKey _xPos, const EKey _xNeg, const EKey _yPos, const EKey _yNeg)
		: InputAction2DAxis(static_cast<Binding>(EKey::UnKnown))
	{
		m_bindings[0] = static_cast<Binding>(_xPos);
		m_bindings[1] = static_cast<Binding>(_xNeg);
		m_bindings[2] = static_cast<Binding>(_yPos);
		m_bindings[3] = static_cast<Binding>(_yNeg);
	}

	void CompositeAction2DAxis::Tick()
	{
		InputAction::Tick();

		const bool xPos = InputModule::IsButtonDown(m_bindings[2]);
		const bool xNeg = InputModule::IsButtonDown(m_bindings[3]);
		const bool yPos = InputModule::IsButtonDown(m_bindings[0]);
		const bool yNeg = InputModule::IsButtonDown(m_bindings[1]);

		const float x = static_cast<float>(xPos) - static_cast<float>(xNeg);
		const float y = static_cast<float>(yPos) - static_cast<float>(yNeg);
		if (m_value != vec2(x, y))
			UpdateValue(x, y);
	}
}

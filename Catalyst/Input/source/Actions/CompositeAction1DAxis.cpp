#include "cainputpch.h"
#include "Actions/CompositeAction1DAxis.h"

#include "InputModule.h"

namespace Catalyst
{
	CompositeAction1DAxis::CompositeAction1DAxis(const EKey _pos, const EKey _neg)
		: InputAction1DAxis(static_cast<Binding>(EKey::UnKnown))
	{
		m_bindings[0] = static_cast<Binding>(_pos);
		m_bindings[1] = static_cast<Binding>(_neg);
	}

	void CompositeAction1DAxis::Tick()
	{
		InputAction::Tick();

		const bool pos = InputModule::IsButtonDown(m_bindings[0]);
		const bool neg = InputModule::IsButtonDown(m_bindings[1]);

		const float val = static_cast<float>(pos) - static_cast<float>(neg);

		UpdateValue(val);
	}
}

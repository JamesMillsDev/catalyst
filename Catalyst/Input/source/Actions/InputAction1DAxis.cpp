#include "cainputpch.h"
#include "Actions/InputAction1DAxis.h"

namespace Catalyst
{
	InputAction1DAxis::InputAction1DAxis(const Binding _binding)
		: InputAction(_binding)
	{

	}

	void InputAction1DAxis::UpdateValue(const float _value)
	{
		m_value = _value;

		for (const auto& cb : m_onPerformed)
			cb->Invoke(m_value);
	}
}
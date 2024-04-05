#include <Catalyst/Input/InputAction1DAxis.hpp>

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

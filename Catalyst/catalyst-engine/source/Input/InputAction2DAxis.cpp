#include <Catalyst/Input/InputAction2DAxis.hpp>

#include <Catalyst/Input/InputModule.hpp>

namespace Catalyst
{
	InputAction2DAxis::InputAction2DAxis(const Binding _binding)
		: InputAction(_binding)
	{

	}

	void InputAction2DAxis::UpdateValue(const float _x, const float _y)
	{
		m_value.x = _x;
		m_value.y = _y;

		for (const auto& cb : m_onPerformed)
			cb->Invoke(m_value);
	}

	void InputAction2DAxis::Tick()
	{
		InputAction::Tick();

		if (m_binding == static_cast<Binding>(EAxis::MouseDelta))
		{
			const vec2 diff = InputModule::GetMousePosition() - InputModule::GetPrevMousePosition();
			UpdateValue(diff.x, diff.y);
		}
		else if (m_binding == static_cast<Binding>(EAxis::MousePos))
		{
			const vec2 pos = InputModule::GetMousePosition();
			if (pos != m_value)
				UpdateValue(pos.x, pos.y);
		}
	}

}
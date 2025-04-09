#include "cainputpch.h"
#include "Actions/InputActionButton.h"

#include "InputModule.h"

namespace Catalyst
{
    InputActionButton::InputActionButton(const Binding _binding)
		: InputAction(_binding)
    {
    }

    void InputActionButton::UpdateValue(const bool _b)
    {
		m_value = _b;
    }

    void InputActionButton::Tick()
    {
		InputAction::Tick();

		const bool pressed = InputModule::IsButtonDown(m_binding);
		const bool wasPressed = InputModule::WasButtonReleased(m_binding);

		if (pressed)
		{
			UpdateValue(true);

			for (const auto& cb : m_onPerformed)
				cb->Invoke(m_value);
		}

		if (!pressed && wasPressed)
		{
			UpdateValue(false);

			for (const auto& cb : m_onCompleted)
				cb->Invoke(m_value);
		}
    }
}

#include <ranges>
#include <Catalyst/Input/Axis2DInputAction.hpp>

namespace Catalyst
{
	Axis2DInputAction::Axis2DInputAction(const EAxisType _axis)
		: m_axis{ _axis }
	{
	}

	void Axis2DInputAction::OnTick()
	{
		switch (m_axis)
		{
		case MousePosition:
			for(const auto& cb : m_performedCallbacks | std::views::values)
			{
				cb->Invoke();
			}

			break;
		case MouseDelta:

			break;

		}
	}
}

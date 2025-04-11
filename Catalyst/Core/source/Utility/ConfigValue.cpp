#include "cacorepch.h"
#include "Utility/ConfigValue.h"

namespace Catalyst
{
	ConfigValue::ConfigValue()
		: m_value{ nullptr }, m_deleter{ nullptr }
	{
	}

	ConfigValue::~ConfigValue()
	{
		m_deleter->Destroy(m_value);
		delete m_deleter;
	}
}
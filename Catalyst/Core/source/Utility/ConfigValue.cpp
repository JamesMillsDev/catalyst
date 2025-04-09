#include "cacorepch.h"
#include "Utility/ConfigValue.h"

namespace Catalyst
{
	ConfigValue::ConfigValue(void* _value)
		: m_value{ _value }
	{

	}

	ConfigValue::ConfigValue(const ConfigValue& _other) = default;

	ConfigValue::ConfigValue(ConfigValue&& _other) noexcept
		: m_value{ _other.m_value }
	{
		_other.m_value = nullptr;
	}

	ConfigValue& ConfigValue::operator=(const ConfigValue& _other) = default;

	ConfigValue& ConfigValue::operator=(ConfigValue&& _other) noexcept
	{
		m_value = _other.m_value;
		_other.m_value = nullptr;

		return *this;
	}

	ConfigValue::~ConfigValue()
	{
		// ReSharper disable once CppDeletingVoidPointer
		delete m_value;  // NOLINT(clang-diagnostic-delete-incomplete)
	}
}
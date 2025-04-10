#include "cacorepch.h"
#include "Utility/ConfigValue.h"

namespace Catalyst
{
<<<<<<< HEAD
	ConfigValue::ConfigValue()
		: m_value{ nullptr }, m_deleter{ nullptr }
	{
=======
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
>>>>>>> develop
	}

	ConfigValue::~ConfigValue()
	{
<<<<<<< HEAD
		m_deleter->Destroy(m_value);
		delete m_deleter;
=======
		// ReSharper disable once CppDeletingVoidPointer
		delete m_value;  // NOLINT(clang-diagnostic-delete-incomplete)
>>>>>>> develop
	}
}
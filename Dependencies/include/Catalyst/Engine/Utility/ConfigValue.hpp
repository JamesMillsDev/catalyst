#pragma once

#include <Catalyst/Catalyst.hpp>

namespace Catalyst
{
	class ConfigValue final
	{
	public:
		DLL ConfigValue() = delete;
		DLL explicit ConfigValue(void* _value);

		DLL ConfigValue(const ConfigValue& _other);
		DLL ConfigValue(ConfigValue&& _other) noexcept;

		DLL ~ConfigValue();

	public:
		template<typename VAL>
		VAL Get();

	public:
		DLL ConfigValue& operator=(const ConfigValue& _other);
		DLL ConfigValue& operator=(ConfigValue&& _other) noexcept;

	private:
		void* m_value;

	};

	template<typename VAL>
	VAL ConfigValue::Get()
	{
		return *static_cast<VAL*>(m_value);
	}
}
//  ***************************************************************
//  ConfigValue - Creation date: 6/9/2024 5:10:17 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"

namespace Catalyst
{
	class DLL ConfigValue final
	{
	public:
		ConfigValue() = delete;
		explicit ConfigValue(void* _value);

		ConfigValue(const ConfigValue& _other);
		ConfigValue(ConfigValue&& _other) noexcept;

		~ConfigValue();

	public:
		template<typename VAL>
		VAL Get();

	public:
		ConfigValue& operator=(const ConfigValue& _other);
		ConfigValue& operator=(ConfigValue&& _other) noexcept;

	private:
		void* m_value;

	};

	template<typename VAL>
	VAL ConfigValue::Get()
	{
		return *static_cast<VAL*>(m_value);
	}
}
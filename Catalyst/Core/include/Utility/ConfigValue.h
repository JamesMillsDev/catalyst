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

#include "PolyDeleter.h"

namespace Catalyst
{
	class DLL ConfigValue
	{
	public:
		ConfigValue();
		template<typename T>
		ConfigValue(T* _value);

		~ConfigValue();

	public:
		template<typename VAL>
		VAL Get();

	private:
		void* m_value;
		DeleterBase* m_deleter;

	};

	template<typename T>
	ConfigValue::ConfigValue(T* _value)
		: m_value{ _value }, m_deleter{ new PolyDeleter<T> }
	{

	}

	template<typename VAL>
	VAL ConfigValue::Get()
	{
		return *static_cast<VAL*>(m_value);
	}
}
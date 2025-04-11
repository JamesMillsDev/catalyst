//  ***************************************************************
//  ShaderPropertyParser - Creation date: 3/25/2025 8:53:56 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"

#include <list>
#include <ranges>
#include <string>
#include <vector>

#include "ShaderProperty.h"

using std::list;
using std::ranges::subrange;
using std::string;
using std::vector;

namespace Catalyst
{
	class DLL ShaderPropertyParser
	{
		friend class ShaderParser;

	private:
		subrange<vector<string>::const_iterator> m_properties;

	private:
		ShaderPropertyParser(const subrange<vector<string>::const_iterator>& _properties);

	private:
		list<ShaderProperty*> Parse();

		ShaderProperty* ParseLine(const string& _line);

		string GetUniformFromString(string& _line);
		string GetDisplayNameFromString(string& _line);
		ShaderProperty::EPropertyType GetTypeFromString(string& _line, string& _customType, bool _hasCustomType, bool& _isArray, uint& _arrSize);
		string GetInitialValueFromString(string& _line, ShaderProperty::EPropertyType _type);

		bool HasTag(string& _line, const string& _tag);
		bool IsArray(string& _line, uint& _size);

	};
}
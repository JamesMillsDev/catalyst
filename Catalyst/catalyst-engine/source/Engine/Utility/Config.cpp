#include <Catalyst/Engine/Utility/Config.hpp>

#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <vector>
#include <Catalyst/Engine/Utility/Color.hpp>
#include <Catalyst/Engine/Utility/ConfigValue.hpp>
#include <Catalyst/Engine/Utility/StringUtils.hpp>
#include <glm/vec2.hpp>
#include <pugixml/pugixml.hpp>
#include <windows.h>

#include "../../../resource.h"

using pugi::xml_parse_result;
using pugi::xml_node;
using pugi::xml_node;

using glm::vec2;

using std::ifstream;
using std::vector;
using std::stringstream;

namespace Catalyst
{
	const char* InvalidValueException::what() const noexcept
	{
		return value.c_str();
	}

	Config::Config()
		: m_doc{ new xml_document }
	{
	}

	Config::~Config()
	{
		for (auto& group : m_data | std::views::values)
		{
			for (const auto& val : group | std::views::values)
				delete val;
		}

		m_data.clear();
	}

	ConfigValue* Config::GetValue(const string& _group, const string& _id)
	{
		if (m_data.contains(_group))
		{
			auto& set = m_data[_group];

			if (set.contains(_id))
				return set[_id];
		}

		return nullptr;
	}

	void Config::Clear()
	{
		for (auto& group : m_data | std::views::values)
		{
			for (auto iter = group.begin(); iter != group.end(); ++iter)
				delete iter->second;
		}

		m_data.clear();
	}

	void Config::Load(bool _initialise)
	{
		if (_initialise && !Initialise())
			return;

		for (const xml_node& category : m_doc->child("Config").child("Categories").children("Category"))
		{
			string categoryTitle = category.attribute("title").as_string();

			for (const xml_node& value : category.children("Value"))
			{
				string valName = value.attribute("name").as_string();
				string valType = value.attribute("type").as_string();

				switch (StringToType(valType))
				{
				case EValType::Int:
					m_data[categoryTitle][valName] = new ConfigValue(new int(value.attribute("value").as_int()));
					break;

				case EValType::Float:
					m_data[categoryTitle][valName] = new ConfigValue(new float(value.attribute("value").as_float()));
					break;

				case EValType::Bool:
					m_data[categoryTitle][valName] = new ConfigValue(new bool(value.attribute("value").as_bool()));
					break;

				case EValType::String:
					m_data[categoryTitle][valName] = new ConfigValue(new string(value.attribute("value").as_string()));
					break;

				case EValType::Vector:
					HandleVector(categoryTitle, valName, value.attribute("value").as_string());
					break;

				case EValType::Color:
					HandleColor(categoryTitle, valName, value.attribute("value").as_string());
					break;

				case EValType::Invalid:
					std::cout << "Type " << valType.c_str() << " was invalid! Make sure you mark the type as one of the following: int, float, bool, string, vector, color" << std::endl;
					break;
				}
			}
		}
	}

	void Config::HandleVector(const string& _category, const string& _name, const string& _value)
	{
		auto converter = [](const string& _val) -> float
			{
				return std::stof(_val);
			};

		const vector<float> values = StringUtils::Split<float>(_value, ',', converter);

		m_data[_category][_name] = new ConfigValue(new vec2{ values[0], values[1] });
	}

	void Config::HandleColor(const string& _category, const string& _name, const string& _value)
	{
		auto converter = [](const string& _val) -> float
			{
				return std::stof(_val);
			};

		const vector<float> values = StringUtils::Split<float>(_value, ',', converter);

		m_data[_category][_name] = new ConfigValue(new Color
			{
				values[0],
				values[1],
				values[2],
				values[3]
			});
	}

	HMODULE GetHandle()
	{
		HMODULE hModule = nullptr;
		GetModuleHandleEx(
			GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
			(LPCTSTR)GetHandle, &hModule);

		return hModule;
	}

	string Config::GetConfigData(const int _id)
	{
		string result;

		const HMODULE handle = GetHandle();
		const HRSRC hRes = FindResource(handle, MAKEINTRESOURCE(_id), MAKEINTRESOURCE(TEXTFILE));

		if (hRes != nullptr)
		{
			const HGLOBAL hData = LoadResource(handle, hRes);
			const DWORD hSize = SizeofResource(handle, hRes);

			if (hData != nullptr)
			{
				const char* hFinal = static_cast<char*>(LockResource(hData));

				result.assign(hFinal, hSize);
			}
		}

		return result;
	}

	bool Config::Initialise() const
	{
		const xml_parse_result result = m_doc->load_string(GetConfigData(CONFIG_FILE).c_str());

		return result;
	}

	EValType Config::StringToType(const string& _type)
	{
		if (_type == "int")
			return EValType::Int;

		if (_type == "float")
			return EValType::Float;

		if (_type == "bool")
			return EValType::Bool;

		if (_type == "string")
			return EValType::String;

		if (_type == "vector")
			return EValType::Vector;

		if (_type == "color")
			return EValType::Color;

		return EValType::Invalid;
	}
}
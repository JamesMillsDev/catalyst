#include "cacorepch.h"
#include "Utility/Config.h"

#include "Utility/ConfigValue.h"
#include "Utility/StringUtils.h"

#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <pugixml/pugixml.hpp>

using pugi::char_t;
using pugi::xml_parse_result;
using pugi::xml_node;

using glm::vec2;
using glm::vec3;
using glm::vec4;

using std::ifstream;
using std::vector;
using std::stringstream;

namespace Catalyst
{
	const char* InvalidValueException::what() const noexcept
	{
		return value.c_str();
	}

	Config::Config(const uint8_t* data, size_t length)
		: m_doc{ new xml_document }, m_content{ data }, m_length{ length }
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

		delete m_doc;
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

	bool Config::TryGetValue(const string& _group, const string& _id, ConfigValue*& _value)
	{
		if (HasValue(_group, _id))
		{
			_value = GetValue(_group, _id);
			return true;
		}

		return false;
	}

	bool Config::HasValue(const string& _group, const string& _id)
	{
		if (m_data.contains(_group))
		{
			const auto& set = m_data[_group];
			return set.contains(_id);
		}

		return false;
	}

	void Config::Clear()
	{
		for (auto& group : m_data | std::views::values)
		{
			for (const auto& val : group | std::views::values)
				delete val;
		}

		m_data.clear();
	}

	void Config::Load()
	{
		if (!Initialise())
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

				case EValType::Vector2:
					HandleVector2(categoryTitle, valName, value.attribute("value").as_string());
					break;

				case EValType::Vector3:
					HandleVector3(categoryTitle, valName, value.attribute("value").as_string());
					break;

				case EValType::Vector4:
					HandleVector4(categoryTitle, valName, value.attribute("value").as_string());
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

	bool Config::Initialise() const
	{
		const xml_parse_result result = m_doc->load_string(reinterpret_cast<const char_t*>(m_content));

		return result;
	}

	void Config::HandleVector2(const string& _category, const string& _name, const string& _value)
	{
		auto converter = [](const string& _val) -> float
			{
				return std::stof(_val);
			};

		const vector<float> values = StringUtils::Split<float>(_value, ',', converter);

		m_data[_category][_name] = new ConfigValue(new vec2{ values[0], values[1] });
	}

	void Config::HandleVector3(const string& _category, const string& _name, const string& _value)
	{
		auto converter = [](const string& _val) -> float
			{
				return std::stof(_val);
			};

		const vector<float> values = StringUtils::Split<float>(_value, ',', converter);

		m_data[_category][_name] = new ConfigValue(new vec3{ values[0], values[1], values[2] });
	}

	void Config::HandleVector4(const string& _category, const string& _name, const string& _value)
	{
		auto converter = [](const string& _val) -> float
			{
				return std::stof(_val);
			};

		const vector<float> values = StringUtils::Split<float>(_value, ',', converter);

		m_data[_category][_name] = new ConfigValue(new vec4{ values[0], values[1], values[2], values[3] });
	}

	void Config::HandleColor(const string& _category, const string& _name, const string& _value)
	{
		auto converter = [](const string& _val) -> float
			{
				return std::stof(_val);
			};

		const vector<float> values = StringUtils::Split<float>(_value, ',', converter);

		m_data[_category][_name] = new ConfigValue(new vec4
			{
				values[0],
				values[1],
				values[2],
				values[3]
			});
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

		if (_type == "vector2")
			return EValType::Vector2;

		if (_type == "vector3")
			return EValType::Vector3;

		if (_type == "vector4")
			return EValType::Vector4;

		if (_type == "color")
			return EValType::Color;

		return EValType::Invalid;
	}
}
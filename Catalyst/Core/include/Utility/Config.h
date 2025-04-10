//  ***************************************************************
//  Config - Creation date: 6/9/2024 5:09:08 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"

#include "ConfigValue.h"

#include <functional>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

using std::is_same;
using std::map;
using std::string;
using std::vector;

namespace pugi
{
	class xml_document;
}

using pugi::xml_document;

namespace Catalyst
{
	struct DLL InvalidValueException final : std::exception
	{
	public:
		string value;

	public:
		explicit InvalidValueException(const string& _value)
			: value("Key not found: " + _value) {}

	public:
		[[nodiscard]] const char* what() const noexcept override;
	};

	enum class EValType : int8_t
	{
		Invalid = -1,
		Int,
		Float,
		Bool,
		String,
		Vector2,
		Vector3,
		Vector4,
		Color
	};

	class DLL Config
	{
		friend class Application;

	public:
		explicit Config(const uint8_t* data, size_t length);
		~Config();

		Config(const Config&) = delete;
		Config(Config&&) = delete;

	public:
		ConfigValue* GetValue(const string& _group, const string& _id);
		bool TryGetValue(const string& _group, const string& _id, ConfigValue*& _value);
		bool HasValue(const string& _group, const string& _id);

		template<typename VAL>
		VAL GetValue(const string& _group, const string& _id);
		template<typename VAL>
		bool TryGetValue(const string& _group, const string& _id, VAL& _value);

		void Load();

	public:
		Config& operator=(const Config&) = delete;
		Config& operator=(Config&&) = delete;

	private:
		xml_document* m_doc;

		map<string, map<string, ConfigValue*>> m_data;

		const uint8_t* m_content;
		size_t m_length;

	private:
		bool Initialise() const;
		void Clear();

		void HandleVector2(const string& _category, const string& _name, const string& _value);
		void HandleVector3(const string& _category, const string& _name, const string& _value);
		void HandleVector4(const string& _category, const string& _name, const string& _value);
		void HandleColor(const string& _category, const string& _name, const string& _value);

	private:
		static EValType StringToType(const string& _type);

	};

	template <typename VAL>
	VAL Config::GetValue(const string& _group, const string& _id)
	{
		if (ConfigValue* value = GetValue(_group, _id))
		{
			return value->Get<VAL>();
		}

		return VAL();
	}

	template <typename VAL>
	bool Config::TryGetValue(const string& _group, const string& _id, VAL& _value)
	{
		if (HasValue(_group, _id))
		{
			_value = GetValue<VAL>(_group, _id);
			return true;
		}

		return false;
	}
}
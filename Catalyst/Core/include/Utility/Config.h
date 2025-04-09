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
		ConfigValue* GetValue(const string& group, const string& id);
		bool TryGetValue(const string& group, const string& id, ConfigValue*& value);
		bool HasValue(const string& group, const string& id);

		template<typename VAL>
		VAL GetValue(const string& group, const string& id);
		template<typename VAL>
		bool TryGetValue(const string& group, const string& id, VAL& value);

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

		void HandleVector2(const string& category, const string& name, const string& value);
		void HandleVector3(const string& category, const string& name, const string& value);
		void HandleVector4(const string& category, const string& name, const string& value);
		void HandleColor(const string& category, const string& name, const string& value);

	private:
		static EValType StringToType(const string& type);

	};

	template <typename VAL>
	VAL Config::GetValue(const string& group, const string& id)
	{
		if (ConfigValue* value = GetValue(group, id))
		{
			return value->Get<VAL>();
		}

		return VAL();
	}

	template <typename VAL>
	bool Config::TryGetValue(const string& group, const string& id, VAL& value)
	{
		if (HasValue(group, id))
		{
			value = GetValue<VAL>(group, id);
			return true;
		}

		return false;
	}
}
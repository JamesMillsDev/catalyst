#pragma once

#include <Catalyst/Catalyst.hpp>
#include <Catalyst/Engine/Utility/ConfigValue.hpp>

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
	struct InvalidValueException final : std::exception
	{
	public:
		string value;

	public:
		explicit InvalidValueException(const string& _value)
			: value("Key not found: " + _value) {}

	public:
		[[nodiscard]] DLL const char* what() const noexcept override;
	};

	enum class EValType : int8_t
	{
		Invalid = -1,
		Int,
		Float,
		Bool,
		String,
		Vector,
		Color
	};

	class Config
	{
		friend class Application;

	public:
		DLL Config();
		DLL ~Config();

		DLL Config(const Config&) = delete;
		DLL Config(Config&&) = delete;

	public:
		DLL class ConfigValue* GetValue(const string& _group, const string& _id);

		template<typename VAL>
		VAL GetValue(const string& _group, const string& _id);

	public:
		DLL Config& operator=(const Config&) = delete;
		DLL Config& operator=(Config&&) = delete;

	private:
		xml_document* m_doc;

		map<string, map<string, class ConfigValue*>> m_data;

	private:
		DLL bool Initialise() const;
		DLL void Clear();
		DLL void Load(bool _initialise = true);

		DLL void HandleVector(const string& _category, const string& _name, const string& _value);
		DLL void HandleColor(const string& _category, const string& _name, const string& _value);

	private:
		DLL static string GetConfigData(int _id);
		DLL static EValType StringToType(const string& _type);

	};

	template <typename VAL>
	VAL Config::GetValue(const string& _group, const string& _id)
	{
		if (ConfigValue* value = GetValue(_group, _id))
			return value->Get<VAL>();

		return VAL();
	}
}
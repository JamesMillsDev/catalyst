#pragma once

#include "Catalyst.h"

#include <functional>
#include <string>
#include <list>
#include <tuple>
#include <unordered_map>

using std::string;
using std::function;
using std::list;
using std::tuple;

namespace Catalyst
{
	class ActorComponent;

	class DLL ComponentRegistry
	{
	public:
		// Static Types
		using FactoryFunction = function<ActorComponent* ()>;

		// Full definition of AutoRegister
		template <typename T>
		struct AutoRegister
		{
		public:
			AutoRegister()
			{
				RegisterTyped<T>();
			}

		public:
			static const AutoRegister<T>& instance;
		};

	public:
		// Static Functions
		static void Register(const string& name, FactoryFunction factory);
		static ActorComponent* Create(const string& name);
		static list<tuple<string, string, FactoryFunction>>& GetRegistry();

	public:
		// Static Template Functions (declarations)
		template <typename T>
		static ActorComponent* CreateTypedInstance();

	public:
		template <typename T>
		static void RegisterTyped();

	public:
		template <typename T>
		static FactoryFunction GetFactory();

	};

	// Template Definitions

	template <typename T>
	ActorComponent* ComponentRegistry::CreateTypedInstance()
	{
		return new T();
	}

	template <typename T>
	void ComponentRegistry::RegisterTyped()
	{
		Register(typeid(T).name(), &CreateTypedInstance<T>);
	}

	template <typename T>
	ComponentRegistry::FactoryFunction ComponentRegistry::GetFactory()
	{
		return FactoryFunction(&CreateTypedInstance<T>);
	}

	// Definition of static instance
	template <typename T>
	inline const typename ComponentRegistry::AutoRegister<T>& ComponentRegistry::AutoRegister<T>::instance =
		[]() -> const AutoRegister<T>&
		{
			static AutoRegister<T> inst;
			return inst;
		}();
} // namespace Catalyst
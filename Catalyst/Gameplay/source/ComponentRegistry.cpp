#include "ComponentRegistry.h"

namespace Catalyst
{
	static list<tuple<string, string, ComponentRegistry::FactoryFunction>> s_registry;

	void ComponentRegistry::Register(const string& name, FactoryFunction factory)
	{
		s_registry.emplace_back(name, name, factory);
	}

	ActorComponent* ComponentRegistry::Create(const string& name)
	{
		for (auto& [type, _, factory] : s_registry)
		{
			if (type == name)
			{
				return factory();
			}
		}

		return nullptr;
	}

	list<tuple<string, string, ComponentRegistry::FactoryFunction>>& ComponentRegistry::GetRegistry()
	{
		return s_registry;
	}

} // namespace Catalyst
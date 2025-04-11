#pragma once

#include "ComponentRegistry.h"

#define GENERATED_COMPONENT(TYPE) \
		static Catalyst::ActorComponent* CreateInstance() { return new TYPE(); }

#define REGISTER_COMPONENT(TYPE) \
	inline static Catalyst::ComponentRegistry::AutoRegister<TYPE> s_ComponentAutoRegister; \
	inline static const void* s_ComponentAnchor = static_cast<const void*>(&s_ComponentAutoRegister)


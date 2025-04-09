#include "cacorepch.h"
#include "IModule.h"

#include "Application.h"

namespace Catalyst
{
	IModule::~IModule() = default;

	void IModule::AddModule(IModule* _module)
	{
		Application::m_app->m_modules.emplace_back(_module);
		_module->Enter();
	}

	void IModule::Tick() { }

	void IModule::Render() { }
}

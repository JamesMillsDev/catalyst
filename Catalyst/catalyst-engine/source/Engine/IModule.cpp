#include <Catalyst/Engine/IModule.hpp>

namespace Catalyst
{
	void IModule::OnInitialise(BaseApplication* _app)
	{
	}

	void IModule::OnShutdown(BaseApplication* _app)
	{
	}

	void IModule::Tick(BaseApplication* _app)
	{
	}

	void IModule::Render(BaseApplication* _app)
	{
	}

	string IModule::Name() const
	{
		return m_name;
	}

	IModule::IModule(string _name)
		: m_name{ std::move(_name) }
	{
	}
}

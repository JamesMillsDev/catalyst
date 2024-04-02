#include <Catalyst/Engine/IModule.hpp>

namespace Catalyst
{
	void IModule::OnInitialise(Application* _app)
	{
	}

	void IModule::OnShutdown(Application* _app)
	{
	}

	void IModule::Tick(Application* _app)
	{
	}

	void IModule::Render(Application* _app)
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

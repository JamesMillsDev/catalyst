#pragma once

#include <Catalyst/Catalyst.hpp>

#include <string>

using std::string;

namespace Catalyst
{
	class DLL IModule
	{
	public:
		virtual void OnInitialise(class BaseApplication* _app);
		virtual void OnShutdown(class BaseApplication* _app);

		virtual void Tick(class BaseApplication* _app);
		virtual void Render(class BaseApplication* _app);

		[[nodiscard]] string Name() const;

	protected:
		IModule(string _name);
		virtual ~IModule() = default;

	private:
		string m_name;

	};
}
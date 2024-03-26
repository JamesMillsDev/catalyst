#pragma once

#include <Catalyst/Catalyst.hpp>

#include <string>

using std::string;

namespace Catalyst
{
	class IModule
	{
		friend class BaseApplication;

	public:
		DLL virtual void OnInitialise(class BaseApplication* _app);
		DLL virtual void OnShutdown(class BaseApplication* _app);

		DLL virtual void Tick(class BaseApplication* _app);
		DLL virtual void Render(class BaseApplication* _app);

		[[nodiscard]] DLL string Name() const;

	protected:
		DLL IModule(string _name);
		virtual ~IModule() = default;

	private:
		string m_name;

	};
}
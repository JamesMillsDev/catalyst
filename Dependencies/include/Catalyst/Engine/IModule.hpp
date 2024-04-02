#pragma once

#include <Catalyst/Catalyst.hpp>

#include <string>

using std::string;

namespace Catalyst
{
	class IModule
	{
		friend class Application;

	public:
		DLL virtual void OnInitialise(class Application* _app);
		DLL virtual void OnShutdown(class Application* _app);

		DLL virtual void Tick(class Application* _app);
		DLL virtual void Render(class Application* _app);

		[[nodiscard]] DLL string Name() const;

	protected:
		DLL IModule(string _name);
		virtual ~IModule() = default;

	private:
		string m_name;

	};
}
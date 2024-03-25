#pragma once

#include <Catalyst/Catalyst.hpp>
#include <Catalyst/Engine/Object.hpp>

namespace Catalyst
{
	class GameInstance : public Object
	{
	private:
		friend class BaseApplication;

	private:
		DLL virtual void OnGameLoaded();
		DLL virtual void OnGameUnloaded();

		DLL virtual void Tick();
		DLL virtual void Render();

	};
}
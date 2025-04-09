#include "Assets/Asset.h"

namespace Catalyst
{
	Asset::Asset(EContentType type)
		: name{ "none" }, path{ "invalid" }, type{ type },
		isLoaded{ false }
	{
	}

	Asset::~Asset()
	{
		if (isLoaded)
		{
			Unload();
		}
	}

	void Asset::Load()
	{
		isLoaded = true;
		OnLoad();
	}

	void Asset::Unload()
	{
		isLoaded = false;
		OnUnload();
	}
}
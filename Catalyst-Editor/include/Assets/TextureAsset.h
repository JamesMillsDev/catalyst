#pragma once

#include "Asset.h"

namespace Catalyst
{
	class Texture;

	class TextureAsset : public Asset
	{
	public:
		Texture* texture;

	public:
		TextureAsset();

	protected:
		virtual void OnLoad() override;
		virtual void OnUnload() override;

	};
}
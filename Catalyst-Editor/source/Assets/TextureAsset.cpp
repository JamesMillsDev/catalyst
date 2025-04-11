#include "Assets/TextureAsset.h"

#include <Rendering/Texture.h>

namespace Catalyst
{
	TextureAsset::TextureAsset()
		: Asset{ EContentType::Texture }, texture { nullptr }
	{
	}

	void TextureAsset::OnLoad()
	{
		texture = new Texture(path.c_str());
	}

	void TextureAsset::OnUnload()
	{
		delete texture;
	}
}
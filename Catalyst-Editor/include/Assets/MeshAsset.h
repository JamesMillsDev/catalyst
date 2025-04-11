#pragma once

#include "Asset.h"

#include <list>

using std::list;

namespace Catalyst
{
	class Mesh;

	class MeshAsset : public Asset
	{
	public:
		list<Mesh*> meshes;

	public:
		MeshAsset();

	protected:
		virtual void OnLoad() override;
		virtual void OnUnload() override;

	};
}
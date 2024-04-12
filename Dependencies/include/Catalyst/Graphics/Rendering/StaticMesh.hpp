#pragma once

#include <Catalyst/Catalyst.hpp>

#include <list>
#include <memory>

using std::list;
using std::shared_ptr;

namespace Catalyst
{
	class StaticMesh
	{
	public:
		DLL StaticMesh();
		DLL ~StaticMesh();

	public:
		DLL void Load(const char* _fileName, bool _flipTextureV = false);

		DLL size_t GetMaterialCount() const;
		DLL shared_ptr<class Material> GetMaterial(size_t _index);

		DLL size_t GetMeshCount() const;
		DLL shared_ptr<class Mesh> GetMesh(size_t _index);

	private:
		list<shared_ptr<class Material>> m_materials;
		list<shared_ptr<class Mesh>> m_meshes;

	};
}

#include <Catalyst/Graphics/Rendering/StaticMesh.hpp>

#include <assimp/cimport.h>
#include <assimp/scene.h>

#include <Catalyst/Graphics/Rendering/Material.hpp>
#include <Catalyst/Graphics/Rendering/Mesh.hpp>

namespace Catalyst
{
	StaticMesh::StaticMesh() = default;

	StaticMesh::~StaticMesh() = default;

	void StaticMesh::Load(const char* _fileName, const bool _flipTextureV)
	{
		const aiScene* scene = aiImportFile(_fileName, 0);

		string path = _fileName;
		path = path.substr(0, path.find_last_of('/'));

		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
			mesh->InitialiseFromAssimp(scene->mMeshes[i], _flipTextureV);
			m_meshes.push_back(mesh);
		}

		for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		{
			shared_ptr<Material> material = std::make_shared<Material>();
			material->LoadFromAssimp(scene->mMaterials[i], path);
			m_materials.push_back(material);
		}
	}

	size_t StaticMesh::GetMaterialCount() const
	{
		return m_materials.size();
	}

	shared_ptr<Material> StaticMesh::GetMaterial(const size_t _index)
	{
		auto iter = m_materials.begin();
		std::advance(iter, _index);

		return *iter;
	}

	size_t StaticMesh::GetMeshCount() const
	{
		return m_meshes.size();
	}

	shared_ptr<Mesh> StaticMesh::GetMesh(const size_t _index)
	{
		auto iter = m_meshes.begin();
		std::advance(iter, _index);

		return *iter;
	}
}

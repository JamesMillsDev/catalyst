#include "Assets/MeshAsset.h"

#include <Rendering/Mesh.h>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

namespace Catalyst
{
	MeshAsset::MeshAsset()
		: Asset{ EContentType::Mesh }
	{
	}

	void MeshAsset::OnLoad()
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);

		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			const aiMesh* mesh = scene->mMeshes[i];

			meshes.emplace_back(Mesh::FromAssimp(mesh, true));
		}
	}

	void MeshAsset::OnUnload()
	{
		for (auto& mesh : meshes)
		{
			delete mesh;
		}

		meshes.clear();
	}
}
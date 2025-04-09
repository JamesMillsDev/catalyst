#include "cagraphicspch.h"
#include "Components/StaticMeshComponent.h"

#include "GraphicsModule.h"
#include "Rendering/Material.h"
#include "Rendering/Mesh.h"
#include "Components/CameraComponent.h"

#include <Actor.h>
#include <ActorTransform.h>

#include <Application.h>

#include <GL/glew.h>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

namespace Catalyst
{
	StaticMeshComponent::StaticMeshComponent()
	{
	}

	StaticMeshComponent::~StaticMeshComponent()
	{
		for (auto& mesh : submeshes)
		{
			delete mesh;
		}

		for (auto& material : materials)
		{
			delete material;
		}

		submeshes.clear();
		materials.clear();
	}

	void StaticMeshComponent::LoadMesh(const string& _mesh)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(_mesh.c_str(), aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);

		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			const aiMesh* mesh = scene->mMeshes[i];

			submeshes.emplace_back(Mesh::FromAssimp(mesh, true));
		}

		for (uint i = 0; i < scene->mNumMaterials; ++i)
		{
			materials.emplace_back(nullptr);
		}
	}

	void StaticMeshComponent::SetMaterial(int _slot, Material* _material)
	{
		if (_slot < 0)
		{
			return;
		}

		if (_slot < static_cast<int>(materials.size()))
		{
			auto iter = materials.begin();
			std::advance(iter, _slot);

			*iter = _material;
		}
	}

	void StaticMeshComponent::Render()
	{
		if (GraphicsModule* module = Application::GetModule<GraphicsModule>())
		{
			if (CameraComponent* mainCam = module->MainCamera())
			{
				if (ActorTransform* transform = Owner()->Transform())
				{
					for (Mesh* mesh : submeshes)
					{
						Material* material = GetMaterialFor(mesh);

						material->Bind();

						(void)material->Set("_ModelMatrix", transform->LocalToWorld());
						(void)material->Set("_ProjectionViewModel", mainCam->ProjectionView());
						(void)material->Set("_CameraPosition", mainCam->Owner()->Transform()->Location());

						mesh->Render();
					}
				}
			}
		}
	}

	Material* StaticMeshComponent::GetMaterialFor(Mesh* mesh)
	{
		auto iter = materials.begin();

		std::advance(iter, mesh->GetMaterialIndex());

		return *iter;
	}
}
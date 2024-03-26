#include <Catalyst/Graphics/Components/StaticMeshComponent.hpp>

#include <Catalyst/Engine/BaseApplication.hpp>
#include <Catalyst/Gameplay/Actors/Actor.hpp>
#include <Catalyst/Gameplay/Actors/Transform.hpp>
#include <Catalyst/Graphics/Camera.hpp>
#include <Catalyst/Graphics/Graphics.hpp>
#include <Catalyst/Graphics/Rendering/Material.hpp>
#include <Catalyst/Graphics/Rendering/Mesh.hpp>
#include <Catalyst/Graphics/Rendering/Shader.hpp>
#include <Catalyst/Graphics/Rendering/StaticMesh.hpp>

namespace Catalyst
{
	StaticMeshComponent::StaticMeshComponent() = default;

	StaticMeshComponent::~StaticMeshComponent()
	{
		ActorComponent::~ActorComponent();

		m_mesh = nullptr;
	}

	void StaticMeshComponent::SetMesh(const shared_ptr<StaticMesh>& _mesh)
	{
		m_mesh = _mesh;
	}

	void StaticMeshComponent::Render()
	{
		ActorComponent::Render();

		if(const Graphics* graphics = BaseApplication::GetModule<Graphics>())
		{
			if(Camera* current = graphics->MainCamera())
			{
				if (m_mesh == nullptr)
					return;

				// We can render here
				for (uint32 i = 0; i < m_mesh->GetMeshCount(); ++i)
				{
					const shared_ptr<Mesh> mesh = m_mesh->GetMesh(i);
					const shared_ptr<Material> material = m_mesh->GetMaterial(mesh->GetMaterialIndex());

					const auto pvm = current->ProjectTransform(GetOwner()->GetTransform()->Global());
					// ReSharper disable CppExpressionWithoutSideEffects
					material->shader->Set("ProjectionViewModel", pvm);
					material->shader->Set("ModelMatrix", GetOwner()->GetTransform()->Global());
					// ReSharper restore CppExpressionWithoutSideEffects

					mesh->Render();
				}
			}
		}
	}
}

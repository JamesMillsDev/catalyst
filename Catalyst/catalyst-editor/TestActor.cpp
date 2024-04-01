#include "TestActor.h"

#include <Catalyst/Graphics/Components/StaticMeshComponent.hpp>
#include <Catalyst/Graphics/Rendering/Material.hpp>
#include <Catalyst/Graphics/Rendering/Shader.hpp>
#include <Catalyst/Graphics/Rendering/StaticMesh.hpp>

using Catalyst::Shader;
using Catalyst::Material;
using Catalyst::StaticMesh;
using Catalyst::StaticMeshComponent;

TestActor::TestActor()
{
	const shared_ptr<StaticMesh> mesh = std::make_shared<StaticMesh>();
	mesh->Load("../../TestProject/Content/SoulSpear/SoulSpear.obj");

	const shared_ptr<StaticMeshComponent> component = CreateComponent<StaticMeshComponent>();
	component->SetMesh(mesh);

	shared_ptr<Shader> shader = std::make_shared<Shader>();
	shader->Load(Shader::EShaderType::Vertex, "../../TestProject/Content/Shaders/normalLit.vert");
	shader->Load(Shader::EShaderType::Fragment, "../../TestProject/Content/Shaders/normalLit.frag");
	shader->Link();

	for(size_t i = 0; i < mesh->GetMaterialCount(); ++i)
	{
		const shared_ptr<Material> material = mesh->GetMaterial(i);
		material->SetShader(shader);
	}
}

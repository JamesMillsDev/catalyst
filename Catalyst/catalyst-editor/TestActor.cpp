#include "TestActor.h"

#include <Catalyst/Gameplay/Actors/Transform.hpp>
#include <Catalyst/Graphics/Components/StaticMeshComponent.hpp>
#include <Catalyst/Graphics/Rendering/Material.hpp>
#include <Catalyst/Graphics/Rendering/Shader.hpp>
#include <Catalyst/Graphics/Rendering/StaticMesh.hpp>

using Catalyst::StaticMeshComponent;
using Catalyst::Material;
using Catalyst::Shader;
using Catalyst::StaticMesh;

TestActor::TestActor()
{
	const shared_ptr<StaticMesh> mesh = std::make_shared<StaticMesh>();
	mesh->Load("../../TestProject/Content/SoulSpear/SoulSpear.obj", true);

	StaticMeshComponent* component = CreateComponent<StaticMeshComponent>();
	component->SetMesh(mesh);

	/*shared_ptr<ShaderProgram> shader = std::make_shared<ShaderProgram>();
	shader->Load(ShaderProgram::EShaderType::Vertex, "../../TestProject/Content/Shaders/normalLit.vert");
	shader->Load(ShaderProgram::EShaderType::Fragment, "../../TestProject/Content/Shaders/normalLit.frag");
	shader->Link();*/
	const shared_ptr<Shader> shader = std::make_shared<Shader>("../../TestProject/Content/Shaders/Standard.shader");

	for(size_t i = 0; i < mesh->GetMaterialCount(); ++i)
	{
		const shared_ptr<Material> material = mesh->GetMaterial(i);
		material->SetShader(shader);

		material->specular = { 1.f, 1.f, 1.f };
	}

	GetTransform()->TRS({ 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
}

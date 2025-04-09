#include "EditorApplication.h"

#include <format>
#include <random>

#include <glm/glm.hpp>

#include <Actor.h>
#include <ActorTransform.h>
#include <GameplayModule.h>
#include <Rendering/Shader.h>
#include <Rendering/Texture.h>
#include <Rendering/Material.h>
#include <Components/LightComponent.h>
#include <Components/StaticMeshComponent.h>

#include "Screen.h"
#include "Debug/Gizmos.h"
#include "Utility/Config.h"
#include "Viewport/ViewportCameraActor.h"

namespace Catalyst
{
	HMODULE GetHandle()
	{
		HMODULE hModule = nullptr;
		GetModuleHandleEx(
			GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
			reinterpret_cast<LPCTSTR>(GetHandle), &hModule);

		return hModule;
	}

	EditorApplication::EditorApplication()
		: m_editorConfig{ nullptr }, m_vpCam{ nullptr }
	{
	}

	EditorApplication::~EditorApplication()
	{
		delete m_editorConfig;
		m_editorConfig = nullptr;
	}

	int EditorApplication::RunEditor()
	{
		AssignInstance(new EditorApplication);

		return Run();
	}

	void EditorApplication::OnOpened()
	{
		Application::OnOpened();

		m_editorConfig->Load();

		if (GameplayModule* module = Application::GetModule<GameplayModule>())
		{
			Actor* globalLight = module->SpawnActor<Actor>();
			LightComponent* light = globalLight->CreateComponent<LightComponent>();
			light->color = { .5f, 0.f, 0.f };
			globalLight->Transform()->SetEulerAngles({ 45, 90, 0 });

			m_vpCam = module->SpawnActor<ViewportCameraActor>();
			m_vpCam->Initialise(m_editorConfig);

			Actor* fatRat = module->SpawnActor<Actor>();
			StaticMeshComponent* mesh = fatRat->CreateComponent<StaticMeshComponent>();

			mesh->LoadMesh("./resources/models/SM_fatrat.fbx");

			string texture = "fatrat";

			auto makeTextureName = [&](string mapId) -> string
				{
					return std::format("resources/textures/T_{}_{}.png", texture, mapId);
				};

			Shader* shader = new Shader("resources/shaders/test.shader");
			Texture* albedoTexture = new Texture(makeTextureName("D").c_str());
			Texture* normalTexture = new Texture(makeTextureName("N").c_str());
			Texture* ormTexture = new Texture(makeTextureName("ORM").c_str());
			Texture* heightTexture = new Texture(makeTextureName("H").c_str());

			Material* material = new Material();
			material->SetShader(shader);

			mesh->SetMaterial(0, material);
			material->SetTexture("_Albedo", albedoTexture);
			material->SetTexture("_NormalMap", normalTexture);
			material->SetTexture("_ORM", ormTexture);
			material->SetTexture("_HeightMap", heightTexture);
		}

		Gizmos::Create(
			m_editorConfig->GetValue<int>("debug", "3d.maxLines"),
			m_editorConfig->GetValue<int>("debug", "3d.maxTris"),
			m_editorConfig->GetValue<int>("debug", "2d.maxLines"),
			m_editorConfig->GetValue<int>("debug", "2d.maxTris")
		);
	}

	void EditorApplication::OnClosed()
	{
		Application::OnClosed();

		Gizmos::Destroy();
	}

	void EditorApplication::Tick()
	{
		m_vpCam->Tick();

		const Color white = { .5f, .5f, .5f, .5f };
		const Color black = { 0, 0, 0, 1 };

		for (int i = 0; i < 201; ++i)
		{
			Gizmos::AddLine({ -100 + i, 0, 100 }, { -100 + i, 0, -100 }, i % 10 == 0 ? white : black);
			Gizmos::AddLine({ 100, 0, -100 + i }, { -100, 0, -100 + i }, i % 10 == 0 ? white : black);
		}
	}

	void EditorApplication::Render()
	{
		if (const Screen* screen = GetScreen())
		{
			Gizmos::Draw(m_vpCam->ProjectionView());
			Gizmos::Draw2D(static_cast<float>(screen->Width()), static_cast<float>(screen->Height()));
		}
	}

	void EditorApplication::GenerateConfigFiles()
	{
		Application::GenerateConfigFiles();

		m_editorConfig = new Config(GetHandle());
	}
}

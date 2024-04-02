#include "EditorApplication.hpp"

#include <ViewportCamera.h>
#include <Catalyst/Engine/Screen.hpp>
#include <Catalyst/Engine/Utility/Config.hpp>
#include <Debug/Gizmos.hpp>
#include <Catalyst/Gameplay/Actors/Actor.hpp>

#include "../TestActor.h"
#include "../TestLight.h"
#include "../TestPointLightActor.h"
#include "Catalyst/Gameplay/GameplayModule.hpp"
#include "Catalyst/Gameplay/Actors/Transform.hpp"
#include "Catalyst/Graphics/Graphics.hpp"
#include "Catalyst/Graphics/Components/LightComponent.hpp"

using Catalyst::Actor;

namespace Catalyst
{
	EditorApplication::EditorApplication(GameInstance* _game, const string& _appPath)
		: Application(_game, _appPath), m_camera{ nullptr }
	{

	}

	void EditorApplication::OnApplicationOpened()
	{
		Gizmos::Create(
			m_config->GetValue<int>("debug", "3d.maxLines"),
			m_config->GetValue<int>("debug", "3d.maxTris"),
			m_config->GetValue<int>("debug", "2d.maxLines"),
			m_config->GetValue<int>("debug", "2d.maxTris")
		);

		m_camera = std::make_shared<ViewportCamera>(
			m_config
		);

		if (Graphics* graphics = GetModule<Graphics>())
		{
			graphics->SetMainCamera(m_camera.get());
		}

		if (GameplayModule* gameplay = GetModule<GameplayModule>())
		{
			gameplay->SpawnActor<TestActor>();
			gameplay->SpawnActor<TestLight>();
			gameplay->SpawnActor<TestPointLightActor>();

			const shared_ptr<TestPointLightActor> actor = gameplay->SpawnActor<TestPointLightActor>();
			actor->SetupLight({ 0, 1, 0, 1 }, 25.f);
			actor->GetTransform()->TRS({ -1.f, 0, 0.f }, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });

			const shared_ptr<TestPointLightActor> actor2 = gameplay->SpawnActor<TestPointLightActor>();
			actor2->SetupLight({ 0, 0, 1, 1 }, 15.f);
			actor2->GetTransform()->TRS({ 0.f, 5.f, -1.f }, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });
		}
	}

	void EditorApplication::OnApplicationClosed()
	{
		Gizmos::Destroy();
	}

	void EditorApplication::Tick()
	{
		m_camera->Tick();

		// Render a simple grid with gizmos
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
		Gizmos::Draw(m_camera->ProjectionView());
		Gizmos::Draw2D(static_cast<float>(m_screen->Width()), static_cast<float>(m_screen->Height()));
	}
}

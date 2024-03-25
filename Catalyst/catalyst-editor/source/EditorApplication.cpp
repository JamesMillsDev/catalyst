#include "EditorApplication.hpp"

#include <Catalyst/Engine/Screen.hpp>
#include <Catalyst/Engine/Utility/Config.hpp>
#include <Catalyst/Graphics/Camera.hpp>
#include <Debug/Gizmos.hpp>

namespace Catalyst
{
	EditorApplication::EditorApplication(GameInstance* _game, const string& _appPath)
		: BaseApplication(_game, _appPath), m_camera{ nullptr }
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

		m_camera = std::make_shared<Camera>(
			m_config->GetValue<float>("viewport", "camera.fov"),
			m_config->GetValue<float>("viewport", "camera.near"),
			m_config->GetValue<float>("viewport", "camera.far")
		);

	}

	void EditorApplication::OnApplicationClosed()
	{
		Gizmos::Destroy();
	}

	void EditorApplication::Tick()
	{
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

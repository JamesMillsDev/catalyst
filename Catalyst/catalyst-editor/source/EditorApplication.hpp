#pragma once

#include <Catalyst/Engine/Application.hpp>
#include <Catalyst/Gameplay/GameInstance.hpp>

namespace Catalyst
{
	class EditorApplication final : public Application
	{
	public:
		EditorApplication(GameInstance* _game, const string& _appPath);

	public:
		void OnApplicationOpened() override;
		void OnApplicationClosed() override;

		void Tick() override;
		void Render() override;

	private:
		shared_ptr<class ViewportCamera> m_camera;

	};

	class EditorGame final : public GameInstance
	{
	public:
		EditorGame() = default;

	};
}
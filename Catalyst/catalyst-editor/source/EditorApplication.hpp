#pragma once

#include <Catalyst/Engine/BaseApplication.hpp>
#include <Catalyst/Gameplay/GameInstance.hpp>

namespace Catalyst
{
	class EditorApplication final : public BaseApplication
	{
	public:
		EditorApplication(GameInstance* _game, const string& _appPath);

	public:
		void OnApplicationOpened() override;
		void OnApplicationClosed() override;

		void Tick() override;
		void Render() override;

	private:
		shared_ptr<class Camera> m_camera;

	};

	class EditorGame final : public GameInstance
	{
	public:
		EditorGame() = default;

	};
}
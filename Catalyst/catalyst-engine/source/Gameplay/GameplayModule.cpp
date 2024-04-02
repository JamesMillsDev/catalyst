#include <Catalyst/Gameplay/GameplayModule.hpp>

namespace Catalyst
{
	GameplayModule::GameplayModule()
		: IModule("Gameplay")
	{
	}

	GameplayModule::~GameplayModule()
	{
		for (auto& actor : m_actors)
		{
			actor->OnEndPlay();
			actor.reset();
		}

		m_actors.clear();
	}

	void GameplayModule::Tick(Application* _app)
	{
		IModule::Tick(_app);

		for (auto& [fnc, actor] : m_actorListChanges)
			std::invoke(fnc, this, actor);

		m_actorListChanges.clear();

		for (const auto& actor : m_actors)
			actor->Tick();
	}

	void GameplayModule::Render(Application* _app)
	{
		IModule::Render(_app);

		for (const auto& actor : m_actors)
			actor->Render();
	}

	void GameplayModule::AddActor(shared_ptr<Actor> _actor)
	{
		_actor->OnBeginPlay();
		m_actors.emplace_back(std::move(_actor));
	}

	void GameplayModule::RemoveActor(shared_ptr<Actor> _actor)
	{
		_actor->OnEndPlay();
		m_actors.remove(_actor);
		_actor.reset();
	}
}

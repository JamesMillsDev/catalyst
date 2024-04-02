#include <Catalyst/Graphics/Graphics.hpp>

#include <Catalyst/Gameplay/Actors/Actor.hpp>
#include <Catalyst/Graphics/Components/LightComponent.hpp>

using std::vector;

namespace Catalyst
{
	Graphics::Graphics()
		: IModule("Graphics"), m_main{ nullptr }
	{
	}

	Camera* Graphics::MainCamera() const
	{
		return m_main;
	}

	void Graphics::SetMainCamera(Camera* _cam)
	{
		m_main = _cam;
	}

	list<LightComponent*> Graphics::Lights()
	{
		return m_lightComponents;
	}

	LightComponent* Graphics::GetLight(const size_t _index)
	{
		auto iter = m_lightComponents.begin();
		std::advance(iter, _index);

		return *iter;
	}

	void Graphics::Tick(Application* _app)
	{
		IModule::Tick(_app);

		for (auto& [fnc, cam] : m_cameraListChanges)
			std::invoke(fnc, this, cam);

		m_cameraListChanges.clear();

		for (auto& [fnc, light] : m_lightListChanges)
			std::invoke(fnc, this, light);

		m_lightListChanges.clear();
	}

	void Graphics::Add(Camera* _camera)
	{
		if (std::ranges::find(m_cameras, _camera) != m_cameras.end())
			return;

		m_cameraListChanges.emplace_back(&Graphics::AddCamera, _camera);
	}

	void Graphics::Remove(Camera* _camera)
	{
		if (std::ranges::find(m_cameras, _camera) == m_cameras.end())
			return;

		m_cameraListChanges.emplace_back(&Graphics::RemoveCamera, _camera);
	}

	void Graphics::Add(LightComponent* _light)
	{
		if (std::ranges::find(m_lightComponents, _light) != m_lightComponents.end())
			return;

		m_lightListChanges.emplace_back(&Graphics::AddLight, _light);
	}

	void Graphics::Remove(LightComponent* _light)
	{
		if (std::ranges::find(m_lightComponents, _light) == m_lightComponents.end())
			return;

		m_lightListChanges.emplace_back(&Graphics::RemoveLight, _light);
	}

	void Graphics::AddLight(LightComponent* _light)
	{
		m_lightComponents.emplace_back(_light);
	}

	void Graphics::RemoveLight(LightComponent* _light)
	{
		m_lightComponents.remove(_light);
	}

	int Graphics::LightCount(bool _includeDirectional) const
	{
		int count = 0;
		for(auto& light : m_lightComponents)
		{
			if(!light->directional || _includeDirectional)
				count++;
		}

		return count;
	}

	void Graphics::AddCamera(Camera* _camera)
	{
		m_cameras.emplace_back(_camera);
	}

	void Graphics::RemoveCamera(Camera* _camera)
	{
		m_cameras.remove(_camera);
	}
}

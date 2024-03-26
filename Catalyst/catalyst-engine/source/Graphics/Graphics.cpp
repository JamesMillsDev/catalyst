#include <Catalyst/Graphics/Graphics.hpp>

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

	void Graphics::Tick(BaseApplication* _app)
	{
		IModule::Tick(_app);

		for (auto& [fnc, cam] : m_cameraListChanges)
			std::invoke(fnc, this, cam);

		m_cameraListChanges.clear();
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

	void Graphics::AddCamera(Camera* _camera)
	{
		m_cameras.emplace_back(_camera);
	}

	void Graphics::RemoveCamera(Camera* _camera)
	{
		m_cameras.remove(_camera);
	}
}

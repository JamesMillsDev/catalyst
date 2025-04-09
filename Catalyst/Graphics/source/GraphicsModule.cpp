#include "cagraphicspch.h"
#include "GraphicsModule.h"

#include <ranges>

namespace Catalyst
{
	GraphicsModule::GraphicsModule()
		: m_main{ nullptr }
	{
		AddModule(this);
	}

	GraphicsModule::~GraphicsModule()
	{
		m_cameraChanges.clear();
		m_renderFeatureChanges.clear();

		m_cameras.clear();
		m_renderFeatures.clear();

		m_main = nullptr;
	}

	CameraComponent* GraphicsModule::MainCamera() const
	{
		return m_main;
	}

	void GraphicsModule::Register(CameraComponent* _camera)
	{
		if(std::ranges::find(m_cameras, _camera) != m_cameras.end())
			return;

		m_cameraChanges.emplace_back(&GraphicsModule::AddCamera, _camera);
	}

	void GraphicsModule::Deregister(CameraComponent* _camera)
	{
		if(std::ranges::find(m_cameras, _camera) == m_cameras.end())
			return;

		m_cameraChanges.emplace_back(&GraphicsModule::RemoveCamera, _camera);
	}

	void GraphicsModule::Register(IRenderFeature* _renderFeature)
	{
		if(std::ranges::find(m_renderFeatures, _renderFeature) != m_renderFeatures.end())
			return;

		m_renderFeatureChanges.emplace_back(&GraphicsModule::AddRenderFeature, _renderFeature);
	}

	void GraphicsModule::Deregister(IRenderFeature* _renderFeature)
	{
		if(std::ranges::find(m_renderFeatures, _renderFeature) == m_renderFeatures.end())
			return;

		m_renderFeatureChanges.emplace_back(&GraphicsModule::RemoveRenderFeature, _renderFeature);
	}

	void GraphicsModule::Enter()
	{
	}

	void GraphicsModule::Exit()
	{
	}

	void GraphicsModule::Tick()
	{
		for(auto& [fnc, cam] : m_cameraChanges)
			std::invoke(fnc, this, cam);

		for(auto& [fnc, feat] : m_renderFeatureChanges)
			std::invoke(fnc, this, feat);

		m_cameraChanges.clear();
		m_renderFeatureChanges.clear();
	}

	void GraphicsModule::Render()
	{
		IModule::Render();
	}

	void GraphicsModule::AddCamera(CameraComponent* _camera)
	{
		if (m_main == nullptr)
		{
			m_main = _camera;
		}
		
		m_cameras.emplace_back(_camera);
	}

	void GraphicsModule::RemoveCamera(CameraComponent* _camera)
	{
		m_cameras.remove(_camera);

		if (m_main == _camera)
		{
			m_main = m_cameras.empty() ? nullptr : m_cameras.front();
		}
	}

	void GraphicsModule::AddRenderFeature(IRenderFeature* _renderFeature)
	{
		m_renderFeatures.emplace_back(_renderFeature);
	}

	void GraphicsModule::RemoveRenderFeature(IRenderFeature* _renderFeature)
	{
		m_renderFeatures.remove(_renderFeature);
	}
}

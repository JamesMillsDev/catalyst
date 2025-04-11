//  ***************************************************************
//  GraphicsModule - Creation date: 6/8/2024 2:36:00 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "IModule.h"
#include "Utility/LINQ.h"

#include <list>

using std::list;
using std::pair;

namespace Catalyst
{
	class CameraComponent;
	class IRenderFeature;

	class DLL GraphicsModule final : public IModule
	{
		typedef void(GraphicsModule::* CameraListChange)(CameraComponent*);
		typedef void(GraphicsModule::* RenderFeatureListChange)(IRenderFeature*);

	public:
		GraphicsModule();

		GraphicsModule(const GraphicsModule&) = delete;
		GraphicsModule(GraphicsModule&&) = delete;

		~GraphicsModule() override;

	public:
		CameraComponent* MainCamera() const;

		void Register(CameraComponent* _camera);
		void Deregister(CameraComponent* _camera);
		void Register(IRenderFeature* _renderFeature);
		void Deregister(IRenderFeature* _renderFeature);

		template<derived<IRenderFeature> FEATURE>
		list<FEATURE*> GetFeatures();

	public:
		GraphicsModule& operator=(const GraphicsModule&) = delete;
		GraphicsModule& operator=(GraphicsModule&&) = delete;

	protected:
		void Enter() override;
		void Exit() override;

		void Tick() override;
		void Render() override;

	private:
		list<pair<CameraListChange, CameraComponent*>> m_cameraChanges;
		list<pair<RenderFeatureListChange, IRenderFeature*>> m_renderFeatureChanges;

		list<CameraComponent*> m_cameras;
		list<IRenderFeature*> m_renderFeatures;

		CameraComponent* m_main;

	private:
		void AddCamera(CameraComponent* _camera);
		void RemoveCamera(CameraComponent* _camera);
		void AddRenderFeature(IRenderFeature* _renderFeature);
		void RemoveRenderFeature(IRenderFeature* _renderFeature);

	};

	template <derived<IRenderFeature> FEATURE>
	list<FEATURE*> GraphicsModule::GetFeatures()
	{
		list<FEATURE*> features;

		for (IRenderFeature* feature : m_renderFeatures)
		{
			FEATURE* f = dynamic_cast<FEATURE*>(feature);
			if (f != nullptr)
			{
				features.emplace_back(f);
			}
		}

		return features;
	}
}

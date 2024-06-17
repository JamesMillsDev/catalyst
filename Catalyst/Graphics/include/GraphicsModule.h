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
	class Camera;
	class IRenderFeature;

	class DLL GraphicsModule final : public IModule
	{
		typedef void(GraphicsModule::* CameraListChange)(Camera*);
		typedef void(GraphicsModule::* RenderFeatureListChange)(IRenderFeature*);

	public:
		GraphicsModule();

		GraphicsModule(const GraphicsModule&) = delete;
		GraphicsModule(GraphicsModule&&) = delete;

		~GraphicsModule() override;

	public:
		Camera* Main() const;

		void Register(Camera* _camera);
		void Deregister(Camera* _camera);
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
		list<pair<CameraListChange, Camera*>> m_cameraChanges;
		list<pair<RenderFeatureListChange, IRenderFeature*>> m_renderFeatureChanges;

		list<Camera*> m_cameras;
		list<IRenderFeature*> m_renderFeatures;

		Camera* m_main;

	private:
		void AddCamera(Camera* _camera);
		void RemoveCamera(Camera* _camera);
		void AddRenderFeature(IRenderFeature* _renderFeature);
		void RemoveRenderFeature(IRenderFeature* _renderFeature);

	};

	template <derived<IRenderFeature> FEATURE>
	list<FEATURE*> GraphicsModule::GetFeatures()
	{
		return LINQ::Select<list<FEATURE*>, list<IRenderFeature*>, IRenderFeature*, FEATURE*>(
			m_renderFeatures, [this](IRenderFeature* _feature) -> FEATURE*
			{
				return dynamic_cast<FEATURE*>(_feature);
			}
		);
	}
}

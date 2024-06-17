#include "cagraphicspch.h"
#include "IRenderFeature.h"

#include "Application.h"
#include "GraphicsModule.h"

namespace Catalyst
{
	IRenderFeature::IRenderFeature()
	{
		if(GraphicsModule* graphics = Application::GetModule<GraphicsModule>())
			graphics->Register(this);
	}

	IRenderFeature::~IRenderFeature()
	{
		if(GraphicsModule* graphics = Application::GetModule<GraphicsModule>())
			graphics->Deregister(this);
	}
}

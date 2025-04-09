#pragma once

#include "Editor.h"

#include <Components/StaticMeshComponent.h>

namespace Catalyst
{
	class StaticMeshComponentEditor : public Editor<StaticMeshComponent>
	{
	protected:
		void OnGui(StaticMeshComponent* _component) override;
		string Title() override;

	private:
		string m_test;

	};
}


#pragma once

#include <Catalyst/Catalyst.hpp>
#include <Catalyst/Gameplay/Actors/ActorComponent.hpp>

#include <memory>

using std::shared_ptr;

namespace Catalyst
{
	class StaticMeshComponent final : public ActorComponent
	{
	public:
		DLL StaticMeshComponent();
		DLL ~StaticMeshComponent() override;

	public:
		DLL void SetMesh(const shared_ptr<class StaticMesh>& _mesh);
		DLL void Render() override;

	private:
		shared_ptr<class StaticMesh> m_mesh;

	};
}

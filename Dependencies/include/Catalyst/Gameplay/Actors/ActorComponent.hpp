#pragma once

#include <Catalyst/Catalyst.hpp>
#include <Catalyst/Engine/Object.hpp>

namespace Catalyst
{
	class ActorComponent : public Object
	{
		friend class Actor;

	protected:
		DLL ActorComponent();
		DLL ~ActorComponent() override;

	protected:
		DLL virtual void OnBeginPlay();
		DLL virtual void OnEndPlay();

		DLL virtual void Tick();
		DLL virtual void Render();

	protected:
		DLL class Actor* GetOwner() const;

	private:
		class Actor* m_owner;

	};
}
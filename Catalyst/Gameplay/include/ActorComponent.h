//  ***************************************************************
//  ActorComponent - Creation date: 6/12/2024 11:13:58 AM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"
#include "Object.h"

namespace Catalyst
{
	class Actor;

	class DLL ActorComponent : public Object
	{
		friend class GameplayModule;
		friend class Actor;

	public:
		Actor* Owner() const;

	public:
		ActorComponent(const ActorComponent&) = delete;
		ActorComponent(ActorComponent&&) = delete;

	public:
		ActorComponent& operator=(const ActorComponent&) = delete;
		ActorComponent& operator=(ActorComponent&&) = delete;

	protected:
		ActorComponent();
		~ActorComponent() override;

	protected:
		virtual void OnBeginPlay();
		virtual void OnEndPlay();
		virtual void Tick();
		virtual void Render();

	private:
		Actor* m_owner;

	};
}
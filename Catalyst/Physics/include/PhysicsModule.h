//  ***************************************************************
//  PhysicsModule - Creation date: 6/8/2024 2:38:00 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "IModule.h"

namespace Catalyst
{
	class DLL PhysicsModule final : public IModule
	{
	public:
		PhysicsModule();

		PhysicsModule(const PhysicsModule&) = delete;
		PhysicsModule(PhysicsModule&&) = delete;

		~PhysicsModule() override;

	public:
		PhysicsModule& operator=(const PhysicsModule&) = delete;
		PhysicsModule& operator=(PhysicsModule&&) = delete;

	public:
		void Enter() override;
		void Exit() override;

	};
}

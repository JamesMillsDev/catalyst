//  ***************************************************************
//  GameplayModule - Creation date: 6/12/2024 11:10:35 AM
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
	class DLL GameplayModule final : public IModule
	{
	public:
		GameplayModule();
		~GameplayModule() override;

	public:
		GameplayModule(const GameplayModule&) = delete;
		GameplayModule(GameplayModule&&) = delete;

	public:
		GameplayModule& operator=(const GameplayModule&) = delete;
		GameplayModule& operator=(GameplayModule&&) = delete;

	protected:
		void Enter() override;
		void Exit() override;

	};
}
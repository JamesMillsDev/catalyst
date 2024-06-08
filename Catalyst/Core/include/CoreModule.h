//  ***************************************************************
//  CoreModule - Creation date: 6/7/2024 2:48:47 PM
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
	class DLL CoreModule final : public IModule
	{
	public:
		CoreModule();

		CoreModule(const CoreModule&) = delete;
		CoreModule(CoreModule&&) = delete;

		~CoreModule() override;

	public:
		CoreModule& operator=(const CoreModule&) = delete;
		CoreModule& operator=(CoreModule&&) = delete;

	public:
		void Enter() override;
		void Exit() override;
	};
}

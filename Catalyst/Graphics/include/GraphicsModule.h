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

namespace Catalyst
{
	class DLL GraphicsModule final : public IModule
	{
	public:
		GraphicsModule();

		GraphicsModule(const GraphicsModule&) = delete;
		GraphicsModule(GraphicsModule&&) = delete;

		~GraphicsModule() override;

	public:
		GraphicsModule& operator=(const GraphicsModule&) = delete;
		GraphicsModule& operator=(GraphicsModule&&) = delete;

	public:
		void Enter() override;
		void Exit() override;

	};
}

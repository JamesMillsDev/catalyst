//  ***************************************************************
//  InputModule - Creation date: 6/8/2024 2:35:47 PM
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
	class DLL InputModule final : public IModule
	{
	public:
		InputModule();

		InputModule(const InputModule&) = delete;
		InputModule(InputModule&&) = delete;

		~InputModule() override;

	public:
		InputModule& operator=(const InputModule&) = delete;
		InputModule& operator=(InputModule&&) = delete;

	public:
		void Enter() override;
		void Exit() override;

	};
}

//  ***************************************************************
//  MathsModule - Creation date: 6/18/2024 3:27:27 PM
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
	class DLL MathsModule final : public IModule
	{
	public:
		MathsModule();
		~MathsModule() override;

		MathsModule(const MathsModule&) = delete;
		MathsModule(MathsModule&&) = delete;

	public:
		MathsModule& operator=(const MathsModule&) = delete;
		MathsModule& operator=(MathsModule&&) = delete;

	protected:
		void Enter() override;
		void Exit() override;

	};
}

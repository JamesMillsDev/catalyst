//  ***************************************************************
//  Material - Creation date: 6/17/2024 2:56:17 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"

namespace Catalyst
{
	class DLL IRenderFeature
	{
	public:
		IRenderFeature();
		virtual ~IRenderFeature();

	public:
		IRenderFeature(const IRenderFeature&) = delete;
		IRenderFeature(IRenderFeature&&) = delete;

	public:
		IRenderFeature& operator=(const IRenderFeature&) = delete;
		IRenderFeature& operator=(IRenderFeature&&) = delete;

	};
}

//  ***************************************************************
//  Material - Creation date: 6/8/2024 3:38:24 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

namespace Catalyst
{
	class Material
	{
	public:
		Material();
		~Material();

		Material(const Material&) = delete;
		Material(Material&&) = delete;

	public:
		Material& operator=(const Material&) = delete;
		Material& operator=(Material&&) = delete;

	protected:

	};
}
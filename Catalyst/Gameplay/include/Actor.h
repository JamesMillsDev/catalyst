//  ***************************************************************
//  Actor - Creation date: 6/12/2024 11:13:31 AM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

namespace Catalyst
{
	class Actor
	{
	public:
		Actor();
		virtual ~Actor();

	public:
		Actor(const Actor&) = delete;
		Actor(Actor&&) = delete;

	public:
		Actor& operator=(const Actor&) = delete;
		Actor& operator=(Actor&&) = delete;

	};
}
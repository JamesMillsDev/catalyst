//  ***************************************************************
//  ActorComponent - Creation date: 6/12/2024 11:13:58 AM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

namespace Catalyst
{
	class ActorComponent
	{
	public:
		ActorComponent();
		virtual ~ActorComponent();

	public:
		ActorComponent(const ActorComponent&) = delete;
		ActorComponent(ActorComponent&&) = delete;

	public:
		ActorComponent& operator=(const ActorComponent&) = delete;
		ActorComponent& operator=(ActorComponent&&) = delete;

	};
}
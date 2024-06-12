//  ***************************************************************
//  ActorTransform - Creation date: 6/12/2024 11:14:26 AM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

namespace Catalyst
{
	class ActorTransform
	{
	public:
		ActorTransform();
		virtual ~ActorTransform();

	public:
		ActorTransform(const ActorTransform&) = delete;
		ActorTransform(ActorTransform&&) = delete;

	public:
		ActorTransform& operator=(const ActorTransform&) = delete;
		ActorTransform& operator=(ActorTransform&&) = delete;

	};
}
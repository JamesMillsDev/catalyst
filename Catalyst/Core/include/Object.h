//  ***************************************************************
//  Object - Creation date: 6/9/2024 5:13:06 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

namespace Catalyst
{
	class Object
	{
	public:
		Object();
		virtual ~Object();

	public:
		const class UUID* ID() const;
		/*DLL class InputManager* GetInputManager() const;
		DLL class EntityManager* GetEntityManager() const;
		DLL class Physics* GetPhysics() const;*/

	private:
		class UUID* m_id;

	};
}
#pragma once

#include <Catalyst/Catalyst.hpp>

namespace Catalyst
{
	struct UUID;

	class Object
	{
	public:
		DLL Object();
		DLL virtual ~Object();

	public:
		DLL const UUID* ID() const;
		/*DLL class InputManager* GetInputManager() const;
		DLL class EntityManager* GetEntityManager() const;
		DLL class Physics* GetPhysics() const;*/

	private:
		UUID* m_id;

	};
}
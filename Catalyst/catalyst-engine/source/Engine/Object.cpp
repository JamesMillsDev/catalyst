#include <Catalyst/Engine/Object.hpp>

#include <Catalyst/Engine/UUID.hpp>

namespace Catalyst
{
	Object::Object()
		: m_id{ new UUID }
	{

	}

	Object::~Object()
	{
		delete m_id;
		m_id = nullptr;
	}

	const UUID* Object::ID() const
	{
		return m_id;
	}
}

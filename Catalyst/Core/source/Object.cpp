#include "cacorepch.h"
#include "Object.h"

#include "UUID.h"

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

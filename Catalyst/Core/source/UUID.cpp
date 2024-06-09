#include "cacorepch.h"
#include "UUID.h"

enum
{
    Groups = 5,
    GroupSize = 5
};

#include <random>
#include <string>

using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
using std::string;

namespace Catalyst
{
    UUID::UUID()
        : m_value{ nullptr }, m_hashValue{ 0 }
    {
        Generate();
        HashUUID();
    }

    UUID::UUID(const UUID& _other) = default;

    UUID::UUID(UUID&& _other) noexcept
        : m_value{ _other.m_value }, m_hashValue{ _other.m_hashValue }
    {
        _other.m_value = nullptr;
        _other.m_hashValue = 0;
    }

    UUID::~UUID()
    {
        delete[] m_value;
        m_value = nullptr;
    }

    const char* UUID::Get() const
    {
        return m_value;
    }

    uint UUID::Hash() const
    {
        return m_hashValue;
    }

    UUID& UUID::operator=(const UUID& _rhs)  // NOLINT(bugprone-unhandled-self-assignment)
    {
        if (*this == _rhs)
            return *this;

        m_value = _rhs.m_value;
        m_hashValue = _rhs.m_hashValue;

        return *this;
    }

    UUID& UUID::operator=(UUID&& _rhs) noexcept
    {
        if (*this == _rhs)
            return *this;

        m_value = _rhs.m_value;
        m_hashValue = _rhs.m_hashValue;

        _rhs.m_value = nullptr;
        _rhs.m_hashValue = 0;

        return *this;
    }

    bool UUID::operator==(const UUID& _other) const
    {
        return strcmp(_other.m_value, m_value) == 0 && _other.m_hashValue == m_hashValue;
    }

    bool UUID::operator!=(const UUID& _other) const
    {
        return !(*this == _other);
    }

    void UUID::Generate()
    {
        m_value = new char[GroupSize * Groups + Groups];

        random_device device;
        mt19937 generator(device());
        uniform_int_distribution dist;

        string value;

        for (ubyte i = 0; i < Groups; i++)
        {
            for (ubyte j = 0; j < GroupSize; j++)
            {
                switch (rand() % 3)  // NOLINT(concurrency-mt-unsafe)
                {
                case 0:
                    dist = uniform_int_distribution(48, 57);
                    break;
                case 1:
                    dist = uniform_int_distribution(65, 90);
                    break;
                case 2:
                    dist = uniform_int_distribution(97, 122);
                    break;
                default:
                    dist = uniform_int_distribution(48, 57);
                    break;
                }

                value += static_cast<char>(dist(generator));
            }

            if (i + 1 < Groups)
                value += '-';
        }

        strcpy_s(m_value, GroupSize * Groups + Groups, value.data());
    }

    void UUID::HashUUID()
    {
        uint hash = 0;
        uint x;
        constexpr uint len = GroupSize * Groups + Groups;

        for (uint i = 0; i < len; i++)
        {
            hash = (hash << 4) + m_value[i];
            if ((x = hash & 0xF0000000) != 0)
	            hash ^= x >> 24;

        	hash &= ~x;
        }

        m_hashValue = hash;
    }
}

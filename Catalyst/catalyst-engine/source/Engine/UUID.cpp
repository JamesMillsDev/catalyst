#include <Catalyst/Engine/UUID.hpp>

enum
{
	GROUPS = 5,
	GROUP_SIZE = 5
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

	uint32 UUID::Hash() const
	{
		return m_hashValue;
	}

	UUID& UUID::operator=(const UUID& _other)
	{
		if (*this == _other)
			return *this;

		m_value = _other.m_value;
		m_hashValue = _other.m_hashValue;

		return *this;
	}

	UUID& UUID::operator=(UUID&& _other) noexcept
	{
		if (*this == _other)
			return *this;

		m_value = _other.m_value;
		m_hashValue = _other.m_hashValue;

		_other.m_value = nullptr;
		_other.m_hashValue = 0;

		return *this;
	}

	bool UUID::operator==(const UUID& _other) const
	{
		return strcmp(_other.m_value, m_value) == 0 && _other.m_hashValue == m_hashValue;
	}

	bool UUID::operator!=(const UUID& _other) const
	{
		return strcmp(_other.m_value, m_value) != 0 || _other.m_hashValue != m_hashValue;
	}

	void UUID::Generate()
	{
		m_value = new char[GROUP_SIZE * GROUPS + GROUPS];

		random_device device;
		mt19937 generator(device());
		uniform_int_distribution dist;

		string value;

		for (ubyte i = 0; i < GROUPS; i++)
		{
			for (ubyte j = 0; j < GROUP_SIZE; j++)
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

			if (i + 1 < GROUPS)
				value += '-';
		}

		strcpy_s(m_value, GROUP_SIZE * GROUPS + GROUPS, value.data());
	}

	void UUID::HashUUID()
	{
		uint32 hash = 0;
		uint32 x;
		constexpr uint32 len = GROUP_SIZE * GROUPS + GROUPS;

		for (uint32 i = 0; i < len; i++)
		{
			hash = (hash << 4) + m_value[i];
			if ((x = hash & 0xF0000000) != 0)
			{
				hash ^= x >> 24;
			}
			hash &= ~x;
		}

		m_hashValue = hash;
	}
}
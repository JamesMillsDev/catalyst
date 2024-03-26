#pragma once

#include <Catalyst/Catalyst.hpp>

namespace Catalyst
{
	struct UUID
	{
	public:
		DLL UUID();
		DLL UUID(const UUID& _other);
		DLL UUID(UUID&& _other) noexcept;

		DLL ~UUID();

	public:
		DLL const char* Get() const;
		DLL uint32 Hash() const;

	public:
		DLL UUID& operator=(const UUID& _other);
		DLL UUID& operator=(UUID&& _other) noexcept;

		DLL bool operator==(const UUID& _other) const;
		DLL bool operator!=(const UUID& _other) const;

	private:
		DLL void Generate();
		DLL void HashUUID();

	private:
		char* m_value;
		uint32 m_hashValue;

	};
}
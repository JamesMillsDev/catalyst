//  ***************************************************************
//  UUID - Creation date: 6/9/2024 5:13:53 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Catalyst.h"

namespace Catalyst
{
	class DLL UUID
	{
	public:
		UUID();
		UUID(const UUID& _other);
		UUID(UUID&& _other) noexcept;

		~UUID();

	public:
		const char* Get() const;
		uint Hash() const;

	public:
		UUID& operator=(const UUID& _rhs);
		UUID& operator=(UUID&& _rhs) noexcept;

		bool operator==(const UUID& _other) const;
		bool operator!=(const UUID& _other) const;

	private:
		void Generate();
		void HashUUID();

	private:
		char* m_value;
		uint m_hashValue;


	};
}
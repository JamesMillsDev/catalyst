//  ***************************************************************
//  Callback - Creation date: 6/9/2024 5:11:55 PM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

namespace Catalyst
{
	template<typename... TYPES>
	class Callback
	{
	public:
		virtual ~Callback() = default;

	public:
		virtual void Invoke(TYPES... _params) = 0;
	};

	template<typename SCOPE, typename... TYPES>
	class VaradicTemplateCallback final : public Callback<TYPES...>
	{
		typedef void(SCOPE::* Function)(TYPES...);

	public:
		VaradicTemplateCallback(SCOPE* _owner, const Function _callback)
			: m_owner{ _owner }, m_callback{ _callback }
		{

		}

	public:
		void Invoke(TYPES... _params) override
		{
			(m_owner->*m_callback)(_params...);
		}

	private:
		SCOPE* m_owner;
		Function m_callback;

	};
}
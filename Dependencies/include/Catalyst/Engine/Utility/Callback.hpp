#pragma once

namespace Catalyst
{
	class Callback
	{
	public:
		virtual ~Callback() = default;

	public:
		virtual void Invoke() = 0;
		virtual void* Context() = 0;

	};

	template<typename CONTEXT, typename... ARGS>
	class TemplateCallback final : public Callback
	{
		typedef void(CONTEXT::*Fnc)(ARGS...);

	public:
		TemplateCallback(CONTEXT* _context, Fnc _fnc);

	public:
		void Invoke() override {}
		void Invoke(ARGS... _args);

		void* Context() override;

	private:
		CONTEXT* m_context;
		Fnc m_callback;

	};

	template <typename CONTEXT, typename ... ARGS>
	TemplateCallback<CONTEXT, ARGS...>::TemplateCallback(CONTEXT* _context, const Fnc _fnc)
		: m_context{ _context }, m_callback{ _fnc }
	{
	}

	template <typename CONTEXT, typename ... ARGS>
	void TemplateCallback<CONTEXT, ARGS...>::Invoke(ARGS... _args)
	{
		(m_context->*m_callback)(_args...);
	}

	template <typename CONTEXT, typename ... ARGS>
	void* TemplateCallback<CONTEXT, ARGS...>::Context()
	{
		return m_context;
	}
}

//  ***************************************************************
//  InputAction - Creation date: 6/18/2024 11:10:19 AM
//  -------------------------------------------------------------
//  License: Uh, check for license.txt or license.md for that?
//
//  ***************************************************************
//  Programmer(s):  James Mills (jcmills.developer@gmail.com)
//  ***************************************************************

#pragma once

#include "Callback.h"
#include "Bindings.h"

#include <list>

using std::list;
using std::pair;

namespace Catalyst
{
	class Action
	{
		friend class InputModule;

	public:
		Action() = default;
		virtual ~Action() = default;

		Action(const Action&) = delete;
		Action(Action&&) = delete;

	public:
		[[nodiscard]] virtual Binding GetBinding() const;

	public:
		Action& operator=(const Action&) = delete;
		Action& operator=(Action&&) = delete;

	protected:
		Binding m_binding;

	protected:
		virtual void UpdateValue(bool _b) {}
		virtual void UpdateValue(float _val) {}
		virtual void UpdateValue(float _x, float _y) {}

		virtual void Tick() {}

	};

	template<typename VALUE>
	class InputAction : public Action
	{
		typedef void(InputAction::* ListUpdate)(Callback<VALUE>*);

		friend class InputSystem;

	public:
		explicit InputAction(Binding _binding);
		~InputAction() override;

		InputAction(const InputAction&) = delete;
		InputAction(InputAction&&) noexcept = delete;

	public:
		VALUE ReadValue();

		template<typename SCOPE>
		Callback<VALUE>* AddPerformedListener(SCOPE* _owner, void(SCOPE::* _callback)(VALUE));
		void RemovePerformedListener(Callback<VALUE>* _callback);

		template<typename SCOPE>
		Callback<VALUE>* AddCompletedListener(SCOPE* _owner, void(SCOPE::* _callback)(VALUE));
		void RemoveCompletedListener(Callback<VALUE>* _callback);

	public:
		InputAction& operator=(const InputAction&) = delete;
		InputAction& operator=(InputAction&&) noexcept = delete;

	protected:
		VALUE m_value;

		list<Callback<VALUE>*> m_onPerformed;
		list<Callback<VALUE>*> m_onCompleted;

	protected:
		void Tick() override;

	private:
		list<pair<Callback<VALUE>*, ListUpdate>> m_updates;

	private:
		void AddPerformedCallback(Callback<VALUE>* _callback);
		void RemovePerformedCallback(Callback<VALUE>* _callback);

		void AddCompletedCallback(Callback<VALUE>* _callback);
		void RemoveCompletedCallback(Callback<VALUE>* _callback);

	};

	template <typename VALUE>
	InputAction<VALUE>::InputAction(const Binding _binding)
	{
		m_binding = _binding;
	}

	template<typename VALUE>
	inline InputAction<VALUE>::~InputAction()
	{
		for (const auto& cb : m_onPerformed)
			delete cb;

		for (const auto& cb : m_onCompleted)
			delete cb;

		m_onPerformed.clear();
		m_onCompleted.clear();
	}

	template<typename VALUE>
	inline VALUE InputAction<VALUE>::ReadValue()
	{
		return m_value;
	}

	inline Binding Action::GetBinding() const
	{
		return m_binding;
	}

	template<typename VALUE>
	template<typename SCOPE>
	inline Callback<VALUE>* InputAction<VALUE>::AddPerformedListener(SCOPE* _owner, void(SCOPE::* _callback)(VALUE))
	{
		Callback<VALUE>* cb = new VaradicTemplateCallback<SCOPE, VALUE>(_owner, _callback);

		m_updates.emplace_back(cb, &InputAction<VALUE>::AddPerformedCallback);

		return cb;
	}

	template<typename VALUE>
	inline void InputAction<VALUE>::RemovePerformedListener(Callback<VALUE>* _callback)
	{
		m_updates.emplace_back(_callback, &InputAction<VALUE>::RemovePerformedCallback);
	}

	template<typename VALUE>
	template<typename SCOPE>
	inline Callback<VALUE>* InputAction<VALUE>::AddCompletedListener(SCOPE* _owner, void(SCOPE::* _callback)(VALUE))
	{
		Callback<VALUE>* cb = new VaradicTemplateCallback<SCOPE, VALUE>(_owner, _callback);

		m_updates.emplace_back(cb, &InputAction<VALUE>::AddCompletedCallback);

		return cb;
	}

	template<typename VALUE>
	inline void InputAction<VALUE>::RemoveCompletedListener(Callback<VALUE>* _callback)
	{
		m_updates.emplace_back(_callback, &InputAction<VALUE>::RemoveCompletedCallback);
	}

	template<typename VALUE>
	inline void InputAction<VALUE>::AddPerformedCallback(Callback<VALUE>* _callback)
	{
		m_onPerformed.emplace_back(_callback);
	}

	template<typename VALUE>
	inline void InputAction<VALUE>::RemovePerformedCallback(Callback<VALUE>* _callback)
	{
		m_onPerformed.remove(_callback);
	}

	template<typename VALUE>
	inline void InputAction<VALUE>::AddCompletedCallback(Callback<VALUE>* _callback)
	{
		m_onCompleted.emplace_back(_callback);
	}

	template<typename VALUE>
	inline void InputAction<VALUE>::RemoveCompletedCallback(Callback<VALUE>* _callback)
	{
		m_onCompleted.remove(_callback);
	}

	template<typename VALUE>
	inline void InputAction<VALUE>::Tick()
	{
		for (auto& [cb, fnc] : m_updates)
			std::invoke(fnc, this, cb);

		m_updates.clear();
	}
}
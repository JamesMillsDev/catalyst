#pragma once

#include <Catalyst/Engine/Callback.hpp>
#include <Catalyst/Input/Bindings.hpp>
#include <list>

using std::list;
using std::pair;

namespace Catalyst
{
	class Action
	{
		friend class InputModule;

	public:
		virtual ~Action() = default;

	public:
		virtual Binding GetBinding() const;

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
		InputAction(Binding _binding);

		~InputAction() override;

	public:
		VALUE ReadValue();

		template<typename SCOPE>
		Callback<VALUE>* AddPerformedListener(SCOPE* _owner, void(SCOPE::* _callback)(VALUE));
		void RemovePerformedListener(Callback<VALUE>* _callback);

		template<typename SCOPE>
		Callback<VALUE>* AddCompletedListener(SCOPE* _owner, void(SCOPE::* _callback)(VALUE));
		void RemoveCompletedListener(Callback<VALUE>* _callback);

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
#include <stdexcept>

using std::runtime_error;

namespace Catalyst
{
	template <typename T>
	TList<T>::TNode::TNode()
		: data{  }, next{ nullptr }, prev{ nullptr }
	{
	}

	template <typename T>
	TList<T>::TNode::TNode(T data)
		: data{ data }, next{ nullptr }, prev{ nullptr }
	{
	}

	template <typename T>
	TList<T>::TIterator::TIterator(pointer node)
		: node{ node }
	{
	}

	template <typename T>
	typename TList<T>::TIterator TList<T>::TIterator::operator++()
	{
		if(node != nullptr)
		{
			return TIterator(node->next);
		}

		return TIterator(nullptr);
	}

	template <typename T>
	typename TList<T>::TIterator TList<T>::TIterator::operator--()
	{
		if (node != nullptr)
		{
			return TIterator(node->prev);
		}

		return TIterator(nullptr);
	}

	template <typename T>
	typename TList<T>::TIterator TList<T>::TIterator::operator+(int count)
	{
		TIterator nullIter = TIterator(nullptr);

		if(node != nullptr)
		{
			TIterator iter = TIterator(node);
			for(int i = 0; i < count; ++i)
			{
				++iter;

				if(iter == nullIter)
				{
					return nullIter;
				}
			}

			return iter;
		}

		return nullIter;
	}

	template <typename T>
	typename TList<T>::TIterator TList<T>::TIterator::operator-(int count)
	{
		TIterator nullIter = TIterator(nullptr);

		if (node != nullptr)
		{
			TIterator iter = TIterator(node);
			for (int i = 0; i < count; ++i)
			{
				--iter;

				if (iter == nullIter)
				{
					return nullIter;
				}
			}

			return iter;
		}

		return nullIter;
	}

	template <typename T>
	bool TList<T>::TIterator::operator==(const TIterator& rhs) const
	{
		return node == rhs.node;
	}

	template <typename T>
	bool TList<T>::TIterator::operator!=(const TIterator& rhs) const
	{
		return node != rhs.node;
	}

	template <typename T>
	typename TList<T>::TIterator::reference TList<T>::TIterator::operator*() const
	{
		return node->data;
	}

	template <typename T>
	typename TList<T>::TIterator::pointer TList<T>::TIterator::operator->() const
	{
		return &node->data;
	}

	template <typename T>
	TList<T>::TList()
		: m_head{ nullptr }, m_tail{ nullptr }, m_count{ 0 }
	{
	}

	template <typename T>
	TList<T>::TList(initializer_list<T> values)
	{
		for (auto iter = values.begin(); iter != values.end(); ++iter)
		{
			Add(*iter);
		}
	}

	template <typename T>
	void TList<T>::Add(T data)
	{
		TNode* newNode = new TNode(data);

		if (m_head == nullptr)
		{
			m_head = newNode;
			m_tail = newNode;
		}
		else
		{
			m_tail->next = newNode;
			newNode->prev = m_tail;

			m_tail = newNode;
		}
	}

	template <typename T>
	void TList<T>::Insert(T data, int index)
	{
		Insert(data, Begin() + index);
	}

	template <typename T>
	void TList<T>::Insert(T data, TIterator iterator)
	{
		if (m_head == nullptr)
		{
			// For some reason you inserted into an empty list
			// so, we will just add it instead
			Add(data);
			return;
		}

		if(iterator == End())
		{
			throw runtime_error("Iterator is out of bounds!");
		}

		TNode* newNode = new TNode(data);

	}

	template <typename T>
	void TList<T>::Remove(T data)
	{
		Remove(IndexOf(data));
	}

	template <typename T>
	void TList<T>::Remove(int index)
	{
		Remove(Begin() + index);
	}

	template <typename T>
	void TList<T>::Remove(TIterator iterator)
	{
	}

	template <typename T>
	bool TList<T>::Contains(T data) const
	{
		TNode* current = m_head;
		while (current)
		{
			if (current->data == data)
			{
				return true;
			}

			current = current->next;
		}

		return false;
	}

	template <typename T>
	bool TList<T>::IsEmpty() const
	{
		return m_head == nullptr && m_tail == nullptr;
	}

	template <typename T>
	int TList<T>::IndexOf(T data) const
	{
		TNode* current = m_head;
		if (current->data == data)
		{
			return 0;
		}

		int index = 0;
		while (current)
		{
			if (current->data == data)
			{
				return index;
			}

			index++;
			current = current->next;
		}

		return -1;
	}

	template <typename T>
	size_t TList<T>::Count() const
	{
		return m_count;
	}

	template <typename T>
	T& TList<T>::At(int index)
	{
		return At(Begin() + index);
	}

	template <typename T>
	T& TList<T>::At(TIterator iterator)
	{
		if (IsEmpty())
		{
			throw runtime_error("List cannot be empty!");
		}

		if (iterator == Begin())
		{
			return m_head->data;
		}

		TIterator iter;
		TNode* current = m_head;
		while (current)
		{
			if (iter++ == iterator)
			{
				return current->data;
			}

			current = current->next;
		}

		throw runtime_error("Index out of bounds!");
	}

	template <typename T>
	typename TList<T>::TNode* TList<T>::Front()
	{
		return m_head;
	}

	template <typename T>
	typename TList<T>::TNode* TList<T>::Back()
	{
		return m_tail;
	}

	template <typename T>
	typename TList<T>::TIterator TList<T>::Begin()
	{
		return TIterator(m_head);
	}

	template <typename T>
	typename TList<T>::TIterator TList<T>::End()
	{
		return TIterator(nullptr);
	}

#pragma region C++ STL Compatibility
	template <typename T>
	typename TList<T>::TIterator TList<T>::begin() const
	{
		return Begin();
	}

	template <typename T>
	typename TList<T>::TIterator TList<T>::end() const
	{
		return End();
	}

	template <typename T>
	bool TList<T>::empty() const
	{
		return IsEmpty();
	}
#pragma endregion

}
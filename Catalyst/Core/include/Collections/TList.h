#pragma once

#include <cstddef>
#include <initializer_list>
#include <iterator>

using std::bidirectional_iterator_tag;
using std::initializer_list;
using std::ptrdiff_t;

namespace Catalyst
{
	template<typename T>
	class TList
	{		
	public:
		struct TNode
		{
		public:
			T data;
			TNode* next;
			TNode* prev;

		public:
			TNode();
			TNode(T data);

		};

		class TIterator
		{
		public:
			using iterator_category = bidirectional_iterator_tag;
			using difference_type = ptrdiff_t;
			using value_type = TNode;
			using pointer = value_type*;
			using reference = value_type*;

		public:
			pointer node;

		public:
			explicit TIterator(pointer node);

		public:
			TIterator operator++();
			TIterator operator--();

			TIterator operator+(int count);
			TIterator operator-(int count);

			bool operator==(const TIterator& rhs) const;
			bool operator!=(const TIterator& rhs) const;

			reference operator*() const;
			pointer operator->() const;

		};

	public:
		TList();
		TList(initializer_list<T> values);

	public:
		void Add(T data);

		void Insert(T data, int index);
		void Insert(T data, TIterator iterator);

		void Remove(T data);
		void Remove(int index);
		void Remove(TIterator iterator);

		bool Contains(T data) const;
		bool IsEmpty() const;

		int IndexOf(T data) const;
		size_t Count() const;

		T& At(int index);
		T& At(TIterator iterator);

		TNode* Front();
		TNode* Back();

		TIterator Begin();
		TIterator End();

#pragma region C++ STL Compatibility
		TIterator begin() const;
		TIterator end() const;
		bool empty() const;
#pragma endregion

	private:
		TNode* m_head;
		TNode* m_tail;

		size_t m_count;

	};

}

#include "TList.inl"
#pragma once

#include "Catalyst.h"
#include <stdexcept>
#include <initializer_list>

using std::initializer_list;
using std::runtime_error;

namespace Catalyst
{
    template<typename T>
    class LinkedList
    {
    public:
        // Node structure to hold data and pointers to previous and next nodes.
        struct Node
        {
        public:
            T data;
            Node* prev;
            Node* next;

            // Constructor to initialize the node with data.
            Node(const T& value)
                : data{ value }, next{ nullptr }, prev{ nullptr }
            {
            }
        };

        // Iterator structure for traversing the LinkedList.
        struct Iterator
        {
        public:
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::forward_iterator_tag;

            Node* node = nullptr;

        public:
            // Default constructor
            Iterator() = default;

            // Constructor that initializes iterator with a node.
            Iterator(Node* _node)
                : node{ _node }
            {
            }

            // Increment operators
            Iterator& operator++()
            {
                if (node != nullptr)
                {
                    node = node->next;
                }

                return *this;
            }

            Iterator operator++(int)
            {
                Iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            // Decrement operators
            Iterator& operator--()
            {
                if (node != nullptr)
                {
                    node = node->prev;
                }

                return *this;
            }

            Iterator operator--(int)
            {
                Iterator tmp = *this;
                --(*this);
                return tmp;
            }

            // Equality operators
            bool operator==(const Iterator& _rhs)
            {
                return node == _rhs.node;
            }

            bool operator!=(const Iterator& _rhs)
            {
                return node != _rhs.node;
            }

            // Dereference operators
            reference operator*()
            {
                return node->data;
            }

            pointer operator->()
            {
                return &node->data;
            }

            // Adding and subtracting operators (move forward or backward by a certain amount)
            Iterator& operator+(const int _shiftAmount)
            {
                Iterator iter = *this;
                for (int i = 0; i < _shiftAmount; ++i)
                {
                    if (iter.node != nullptr)
                    {
                        iter.node = iter.node->next;
                    }
                }
                return iter;
            }

            Iterator& operator-(const int _shiftAmount)
            {
                Iterator iter = *this;
                for (int i = _shiftAmount; i > 0; --i)
                {
                    if (iter.node != nullptr)
                    {
                        iter.node = iter.node->prev;
                    }
                }
                return iter;
            }
        };

    public:
        // Default constructor
        LinkedList()
	        : m_head{ nullptr }, m_tail{ nullptr }
        {
	        
        }

        // Constructor from initializer list
        LinkedList(initializer_list<T> values)
            : m_head{ nullptr }, m_tail{ nullptr }
        {
            for (auto iter = values.begin(); iter != values.end(); iter++)
            {
                Add(*iter);
            }
        }

        LinkedList(const LinkedList& other) = delete;
        LinkedList(LinkedList&& other) = delete;

        ~LinkedList()
        {
            Clear(); // Clean up by clearing all nodes
        }

    public:
        // Adds a new node with value at the end
        void Add(const T& value)
        {
            Node* node = new Node(value);
            m_count++;

            if (IsEmpty())
            {
                m_head = node;
                m_tail = node;
            }
            else
            {
                m_tail->next = node;
                node->prev = m_tail;
                m_tail = node;
            }
        }

        // Removes node with specific value
        void Remove(const T& value)
        {
            Remove(Find(value));
        }

        // Removes node at specific index
        void Remove(int index)
        {
            Remove(begin() + index);
        }

        // Removes node at specific iterator
        void Remove(Iterator iter)
        {
            if (iter == end())
            {
                throw runtime_error("Iterator out of range!");
            }

            if (m_count == 1 && iter != end())
            {
                m_head = nullptr;
                m_tail = nullptr;
                delete iter.node;
                iter.node = nullptr;
                m_count--;
                return;
            }

            if (iter.node != m_head)
            {
                iter.node->prev->next = iter.node->next;
            }
            else
            {
                iter.node->next->prev = nullptr;
                m_head = iter.node->next;
            }

            if (iter.node != m_tail)
            {
                iter.node->next->prev = iter.node->prev;
            }
            else
            {
                iter.node->next->prev = nullptr;
                m_tail = iter.node->prev;
            }

            delete iter.node;
        }

        // Inserts node at specific index
        void Insert(int index, const T& value)
        {
            Iterator iter = begin() + index;
            Node* newNode = new Node(value);
            newNode->next = iter.node;

            if (iter != nullptr)
            {
                newNode->prev = iter.node->prev;

                if (iter.node->prev != nullptr)
                {
                    iter.node->prev->next = newNode;
                }

                iter.node->prev = newNode;
            }

            if (m_head == iter.node)
            {
                m_head = newNode;
            }

            if (m_tail == iter.node)
            {
                m_tail = newNode;
            }

            m_count++;
        }

        // Inserts node at specific iterator position
        void Insert(Iterator iter, const T& value)
        {
            Node* newNode = new Node(value);
            newNode->next = iter.node;

            if (iter != nullptr)
            {
                newNode->prev = iter.node->prev;

                if (iter.node->prev != nullptr)
                {
                    iter.node->prev->next = newNode;
                }

                iter.node->prev = newNode;
            }

            if (m_head == iter.node)
            {
                m_head = newNode;
            }

            if (m_tail == iter.node)
            {
                m_tail = newNode;
            }

            m_count++;
        }

        // Finds a node with specific value
        Iterator Find(const T& value)
        {
            if (IndexOf(value) == -1)
            {
                return end();
            }

            return { value };
        }

        // Gets the index of the node with specific value
        int IndexOf(const T& value)
        {
            int index = 0;
            for (Node* node = Front(); node != nullptr; node = node->next)
            {
                if (node->data == value)
                {
                    return index;
                }

                index++;
            }

            return -1;
        }

        // Gets the value at specific index
        T& At(int index)
        {
            if (m_count == 0)
            {
                throw runtime_error("Linked list is empty!");
            }

            if (index >= m_count - 1)
            {
                throw runtime_error("Index is out of range!");
            }

            Node* current = m_head;
            for (int i = 0; i != index; ++i)
            {
                current = current->next;
            }

            return current->data;
        }

        // Gets the value at specific iterator position
        T& At(Iterator iter)
        {
            if (m_count == 0)
            {
                throw runtime_error("Linked list is empty!");
            }

            if (iter == end())
            {
                throw runtime_error("Iterator is out of range!");
            }

            Iterator i = begin();
            for (i; i != iter; iter++) {}

            return *i;
        }

        // Checks if the list contains a node with the value
        bool Contains(const T& value)
        {
            return IndexOf(value) != -1;
        }

        // Checks if the list is empty
        bool IsEmpty()
        {
            return m_head == nullptr && m_tail == nullptr;
        }

        // Clears the entire list
        void Clear()
        {
            while (!IsEmpty())
            {
                Remove(0);
            }
        }

        // Returns the number of elements in the list
        size_t Count() const
        {
            return m_count;
        }

        // Returns the first node
        Node* Front()
        {
            return m_head;
        }

        // Returns the last node
        Node* Back()
        {
            return m_tail;
        }

        // Returns an iterator to the first element
        Iterator begin() const
        {
            return Iterator{ m_head };
        }

        // Returns an iterator to the end of the list
        Iterator end() const
        {
            return Iterator{ nullptr };
        }

    public:
        // Indexing operator for accessing nodes by index
        T& operator[](int index)
        {
            return At(index);
        }

        // Indexing operator for accessing nodes by iterator
        T& operator[](Iterator iter)
        {
            return At(iter);
        }

        LinkedList& operator=(const LinkedList& other) = delete;
        LinkedList& operator=(LinkedList&& other) = delete;

    private:
        // Head of the linked list
        Node* m_head = nullptr;
        // Tail of the linked list
        Node* m_tail = nullptr;
        // Number of nodes in the linked list
        size_t m_count = 0;

    };
}

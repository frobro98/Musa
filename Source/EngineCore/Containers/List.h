// Copyright 2020, Nathan Blane

#pragma once

#include "PlatformDefinitions.h"

template <typename T>
class List
{
private:
	struct ListNode
	{
		ListNode* next = nullptr;
		ListNode* prev = nullptr;
		T item;
	};

public:
	List() = default;

	~List()
	{
		while (listSize > 0)
		{
			RemoveIndex(0);
		}
	}

	template <class Elem>
	void Add(Elem&& val)
	{
		AddNode(std::forward<Elem>(val));
		++listSize;
	}

	void RemoveIndex(uint32 index)
	{
		Assert(listSize > 0);
		Assert(index < listSize);
		auto node = FindNode(index);
		if (listHead == node)
		{
			listHead = node->next;
		}
		if (listTail == node)
		{
			listTail = node->prev;
		}
		RemoveNode(node);
		delete node;
		--listSize;
	}

	void RemoveValues(const T& val)
	{
		Assert(listSize > 0);

		--listSize;
	}

private:
	template <class Elem>
	void AddNode(Elem&& val)
	{
		ListNode* node = new ListNode;
		node->item = std::forward<Elem>(val);

		// Seeding the list
		if (listHead == nullptr)
		{
			listHead = node;
			listTail = listHead;
		}
		else
		{
			listTail->next = node;
			node->prev = listTail;
			listTail = node;
		}
	}

	void RemoveNode(ListNode* node)
	{
		if (node->prev != nullptr)
		{
			node->prev->next = node->next;
		}
		if (node->next != nullptr)
		{
			node->next->prev = node->prev;
		}
	}

	ListNode* FindNode(uint32 i)
	{
		uint32 count = 0;
		auto current = listHead;
		while (current != nullptr)
		{
			if (count == i)
			{
				break;
			}
			++count;
			current = current->next;
		}
		return current;
	}

public:
	// Finds first location of val
	int32 FindFirst(const T& val)
	{
		int32 count = 0;
		auto current = listHead;
		while (current != nullptr)
		{
			if (current->item == val)
			{
				return count;
			}

			++count;
			current = current->next;
		}

		return -1;
	}

	int32 FindLast(const T& val)
	{
		int32 count = listSize-1;
		auto current = listTail;
		while (current != nullptr)
		{
			if (current->item == val)
			{
				return count;
			}

			++count;
			current = current->prev;
		}

		return -1;
	}

public:
	class Iterator
	{
	public:
		Iterator(ListNode* node)
			: current(node)
		{
		}

		Iterator& operator++()
		{
			if (current != nullptr)
			{
				current = current->next;
			}
			return *this;
		}
		bool operator!=(Iterator& other)
		{
			return current != other.current;
		}

		T& operator*()
		{
			return current->item;
		}

	private:
		ListNode* current = nullptr;
	};

	class ConstIterator
	{
	public:
		ConstIterator(const ListNode* node)
			: current(node)
		{
		}

		ConstIterator operator++()
		{
			if (current != nullptr)
			{
				current = current->next;
			}
			return *this;
		}
		bool operator!=(const ConstIterator& other) const
		{
			return current != other.current;
		}

		const T& operator*() const
		{
			return current->item;
		}

	private:
		const ListNode* current = nullptr;
	};


	Iterator begin(){ return Iterator(listHead); }
	Iterator end() { return Iterator(nullptr); }
	ConstIterator begin() const { return ConstIterator(listHead); }
	ConstIterator end() const { return ConstIterator(nullptr); }

private:
	ListNode* listHead = nullptr;
	ListNode* listTail = nullptr;
	uint32 listSize = 0;
};
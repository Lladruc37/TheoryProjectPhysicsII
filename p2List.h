#ifndef __P2LIST_H__
#define __P2LIST_H__

#include "Globals.h"

// Contains items from double linked list
template<class tdata>
struct p2List_item
{
	tdata data;
	p2List_item<tdata>* next;
	p2List_item<tdata>* prev;

	inline p2List_item(const tdata& newData)
	{
		data = newData;
		next = prev = NULL;
	}

	~p2List_item()
	{}
};

// Manages a double linked list
template<class tdata>
class p2List
{

private:

	p2List_item<tdata>* start;
	p2List_item<tdata>* end;
	uint size;

public:

	// Constructor
	inline p2List()
	{
		start = end = NULL;
		size = 0;
	}

	// Destructor
	~p2List()
	{
		Clear();
	}

	p2List_item<tdata>* GetFirst() const
	{
		return start;
	}

	p2List_item<tdata>* GetLast() const
	{
		return end;
	}

	// Get Size
	uint Count() const
	{
		return size;
	}

	// Add new item
	p2List_item<tdata>* Add(const tdata& item)
	{
		p2List_item<tdata>* dataItem;
		dataItem = new p2List_item < tdata >(item);

		if (start == NULL)
		{
			start = end = dataItem;
		}
		else
		{
			dataItem->prev = end;
			end->next = dataItem;
			end = dataItem;
		}
		++size;
		return(dataItem);
	}

	// Find by index (returns true if found and false if not)
	bool At(uint index, tdata& data) const
	{
		bool ret = false;
		uint i = 0;
		p2List_item<tdata>* pData = start;

		for (uint i = 0; i < index && pData != NULL; ++i)
			pData = pData->next;

		if (pData != NULL)
		{
			ret = true;
			data = pData->data;
		}

		return ret;
	}

	// Deletes an item from the list
	bool Del(p2List_item<tdata>* item)
	{
		if (item == NULL)
		{
			return (false);
		}

		// Now reconstruct the list
		if (item->prev != NULL)
		{
			item->prev->next = item->next;

			if (item->next != NULL)
			{
				item->next->prev = item->prev;
			}
			else
			{
				end = item->prev;
			}
		}
		else
		{
			if (item->next)
			{
				item->next->prev = NULL;
				start = item->next;
			}
			else
			{
				start = end = NULL;
			}
		}

		delete item;
		--size;
		return(true);
	}

	// Destroy and free all memory
	void Clear()
	{
		p2List_item<tdata>* pData;
		p2List_item<tdata>* pNext;
		pData = start;

		while (pData != NULL)
		{
			pNext = pData->next;
			delete (pData);
			pData = pNext;
		}

		start = end = NULL;
		size = 0;
	}

	// returns the first apperance of data as index (-1 if not found)
	int Find(const tdata& data)
	{
		p2List_item<tdata>* tmp = start;
		int index = 0;

		while (tmp != NULL)
		{
			if (tmp->data == data)
				return(index);

			++index;
			tmp = tmp->next;
		}
		return (-1);
	}


	// returns the first apperance (node) of data (NULL if not found)
	p2List_item<tdata>* FindNode(const tdata& data)
	{
		p2List_item<tdata>* tmp = start;

		while (tmp != NULL)
		{
			if (tmp->data == data)
				return(tmp);
			tmp = tmp->next;
		}

		return (NULL);
	}
};
#endif // __P2LIST_H__
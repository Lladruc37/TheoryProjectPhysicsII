// ----------------------------------------------------
// Array that resizes dynamically   -------------------
// ----------------------------------------------------

#ifndef __P2DYNARRAY_H__
#define __P2DYNARRAY_H__

#include <assert.h>

#define DYN_ARRAY_BLOCK_SIZE 16

template<class VALUE>
class p2DynArray
{
private:

	VALUE* data;
	uint memCapacity;
	uint numElements;

public:

	// Constructors
	p2DynArray() : memCapacity(0), numElements(0), data(NULL)
	{
		Alloc(DYN_ARRAY_BLOCK_SIZE);
	}

	p2DynArray(uint capacity) : memCapacity(0), numElements(0), data(NULL)
	{
		Alloc(capacity);
	}

	p2DynArray(const p2DynArray& array) : memCapacity(0), numElements(0), data(NULL)
	{
		Alloc(array.numElements);

		for (uint i = 0; i < array.numElements; ++i)
			PushBack(array.data[i]);
	}

	// Destructor
	~p2DynArray()
	{
		delete[] data;
	}

	// Operators
	VALUE& operator[](uint index)
	{
		assert(index < numElements);
		return data[index];
	}

	const VALUE& operator[](uint index) const
	{
		assert(index < numElements);
		return data[index];
	}

	const p2DynArray<VALUE>& operator=(const p2DynArray<VALUE>& other)
	{
		Alloc(other.numElements);

		for (uint i = 0; i < other.numElements; ++i)
			PushBack(other.data[i]);

		return *this;
	}

	// Data Management
	void PushBack(const VALUE& element)
	{
		if (numElements >= memCapacity)
		{
			Alloc(memCapacity + DYN_ARRAY_BLOCK_SIZE);
		}

		data[numElements++] = element;
	}

	bool Pop(VALUE& value)
	{
		if (numElements > 0)
		{
			value = data[--numElements];
			return true;
		}
		return false;
	}

	void Clear()
	{
		numElements = 0;
	}

	bool Insert(const VALUE& element, unsigned int position)
	{
		if (position > numElements)
			return false;

		if (position == numElements)
		{
			PushBack(element);
			return true;
		}

		if (numElements + 1 > memCapacity)
			Alloc(memCapacity + DYN_ARRAY_BLOCK_SIZE);

		for (uint i = numElements; i > position; --i)
		{
			data[i] = data[i - 1];
		}

		data[position] = element;
		++numElements;

		return true;
	}

	VALUE* At(uint index)
	{
		VALUE* result = NULL;

		if (index < numElements)
			return result = &data[index];

		return result;
	}

	const VALUE* At(uint index) const
	{
		VALUE* result = NULL;

		if (index < numElements)
			return result = &data[index];

		return result;
	}

	// Utils
	uint GetCapacity() const
	{
		return memCapacity;
	}

	uint Count() const
	{
		return numElements;
	}

private:

	// Private Utils
	void Alloc(uint mem)
	{
		VALUE* tmp = data;

		memCapacity = mem;
		data = new VALUE[memCapacity];

		if (numElements > memCapacity)
			numElements = memCapacity;

		if (tmp != NULL)
		{
			for (uint i = 0; i < numElements; ++i)
				data[i] = tmp[i];

			delete[] tmp;
		}
	}
};

#endif // __P2DYNARRAY_H__
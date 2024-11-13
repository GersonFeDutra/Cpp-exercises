#pragma once
#include <assert.h>
#include <stdlib.h>

template <typename T> class ArrayList
{
	private:
	T *data = nullptr;
	size_t size = 0;
	size_t capacity = 0;

	private:
	void grow()
	{
		_expand(capacity + capacity / 2);
	}

	void _expand(size_t to)
	{
		assert(to >= capacity);

		T *new_data = new T[to];
		copy(*this, *new_data);

		capacity = to;
		delete[] data;
		data = new_data;
	}

	public:
	ArrayList(size_t base_capacity)
	{
	}

	ArrayList()
	{
		capacity = 2;
	}

	void expand(size_t to)
	{
		if (to <= capacity)
			return;
		else
			_expand(to);
	}

	void copy(T &from, T &to)
	{
		assert(from.capacity <= to.capacity);

		T *p = from.data;
		T *q = to.data;
		T *end = from.data + from.capacity;

		while (p != end)
			*p++ = *q++;
	}
};

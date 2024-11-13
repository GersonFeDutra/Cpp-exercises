#include <assert.h>
#include <stddef.h>

namespace ds {

template <typename T, size_t S> class Array
{
public:
	constexpr size_t Size() { return S; }

	T &operator[](size_t index)
	{
		assert(index < S);
		return data[index];
	}

	constexpr const T &operator[](const size_t index) const
	{
		assert(index < S);
		return data[index];
	}

	//T *Data() { return data; }
	//const T *Data() const { return data; }

private:
	T data[S];
};

}

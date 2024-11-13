#include "Array.hpp"
#include <array>
#include <iostream>

int main()
{
	using namespace ds;

	constexpr int SIZE = 5;
	int array[SIZE];

	int size = 5;
	int *heapArray = new int[size];

	std::array<int, 10> collection;
	size_t length = collection.size();

	int c = 0;
	for (int &item : collection) {
		item = c++;
		std::cout << item << std::endl;
	}

	Array<int, 5> data;
	// memset(data.Data(), 0, data.Size() * sizeof(int)); // initialize

	for (size_t i = 0; i < data.Size(); i++) {
		data[i] = c++;
		std::cout << data[i] << std::endl;
	}
}

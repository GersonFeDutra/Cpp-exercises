//import Queue;   //get Queue’s interface // import support is not complete
#include "queue.hpp"
#include <iostream>

#include <ctime>
#include <random>
#define MAX 100
#define randomize() std::srand(std::time(nullptr))


int main()
{
	randomize();

	Queue v = {(double)(std::rand() % MAX), (double)(std::rand() % MAX), (double)(std::rand() % MAX), (double)(std::rand() % MAX), (double)(std::rand() % MAX), (double)(std::rand() % MAX), };
	std::cout << v;

	while (std::cin) {
		std::cin >> v;
		std::cout << v;
	}

	std::cout << '\n';
}

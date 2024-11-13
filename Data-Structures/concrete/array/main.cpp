#include "array.hpp"
#include <iostream>


int main() {
    using namespace cds;

    { // Scope test
        constexpr size_t n = 5;
        Array<n> arr;
        std::cout << arr << std::endl;

        for (size_t i = 0; i < n; ++i)
            arr[i] = i * 1.1;

        std::cout << arr << std::endl;
    } // Scope test

    return 0;
}

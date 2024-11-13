#pragma once
#include <assert.h>
#include <stddef.h>
#include <iostream>
#include <memory>
#include <cstring> // For std::memcpy
#include <cstdlib> // For alloca

namespace cds { // Concrete Data Structures namespace

template <size_t S> class Array {
private:
    double data[S];

public:
    Array() {
        // Initialize the array with zeros
        for (size_t i = 0; i < S; ++i)
            data[i] = 0.0;
#ifdef DEBUG
        std::cout << *this;
#endif
    }

    Array(std::initializer_list<double> il) {
       std::copy(il.begin(), il.end(), data);
    }

    Array(const Array&copy) = default;
    Array& operator=(const Array&copy) = default;
    Array(Array&&move) = default;
    Array& operator=(Array&&move) = default;

#ifdef DEBUG
    ~Array() {
        for (size_t i = 0; i < S; ++i)
            data[i] = -1.0;
        std::cout << *this;
    }
#endif

    double& operator[](size_t index) {
        if (index >= S)
            throw std::out_of_range("Index out of bounds");

        return data[index];
    }

    const double& operator[](size_t index) const {
        if (index >= S)
            throw std::out_of_range("Index out of bounds");

        return data[index];
    }

    size_t size() const { return S; }

    friend std::ostream& operator<<(std::ostream& os, const Array& c) {
        for (size_t i = 0; i < S; ++i)
            os << c.data[i] << " ";
        os << std::endl;

        return os;
    }

    // TODO -> Add iterator
};

}

#include "matrix.hpp"
#include <algorithm>
#include <iostream>
#include <memory>

template <typename __t, int __m, int __n>

void print_square(Matrix<__t, __m, __n> mat) {
  std::cout << mat << " is" << (mat.isSquare() ? " " : " not ") << "square"
            << std::endl;
}

int main() {
  // Matrix<int, 4, 5> mat;
  // mat.fill([](int i, int j) { return 2 * (i + 1) + 3 * (j + 1); });
  // std::cout << mat << std::endl;
  //
  // Matrix<int, 4, 4> mat4;
  // mat4.fill([](int i, int j) { return -(i + 1) + 4 * (j + 1); });
  // std::cout << mat4 << std::endl;
  //
  // auto m = std::unique_ptr<Matrix<int, 4, 5>>(mat4.mul(mat));
  // std::cout << *m << std::endl;

  // Matrix2<int> a = {{2, 0}, {6, 7}};
  // std::cout << a << std::endl;
  //
  // Matrix2<int> b = {{0, 4}, {2, -8}};
  // std::cout << b << std::endl;
  //
  // // TODO: make the methods coerce to correct type
  // auto m1 = std::unique_ptr<Matrix2<int>>(a.mul(b));
  // auto m2 = std::unique_ptr<Matrix2<int>>(b.mul(a));
  // *m1 -= *m2;
  //
  // std::cout << *m1 << std::endl;
  //
  // std::cout << "transposing" << std::endl;
  // Matrix4<int> mx4 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14,
  // 15, 16}}; std::cout << mx4 << std::endl; mx4.transpose(); std::cout << mx4
  // << std::endl;

  // Matrix2<int> a = {{2, 3}, {-5, 9}}, b = {{4, -5}, {3, 1}},
  //              c = {{6, 2}, {7, -8}};
  // std::cout << a + b << std::endl;
  // std::cout << a + c << std::endl;
  // std::cout << c - a << std::endl;
  // std::cout << a * -2 << std::endl;
  // std::cout << b * 6 << std::endl;
  // std::cout << -3 * c << std::endl;

  // Matrix<int, 4, 2> a = {{1, -2}, {3, 1}, {7, -4}, {5, 9}};
  // Matrix<int, 2, 4> b = {{1, 3, -5, 7}, {6, 2, -8, 3}};
  // std::cout << a * b << std::endl;
  // std::cout << b * a << std::endl;
}

#pragma once
#include <array>
#include <assert.h>
#include <iostream>

// Atenção: Os operadores binários +,-,* criam e copieam novos dados
template <class T, size_t M, size_t N> class Matrix {
protected:
  // std::array<std::array<T, N>, M> matrix;
  T matrix[M][N];

  template <size_t _N, size_t _P>

  // TODO -> usar algoritmo otimizado
  void _multiply(const Matrix<T, _N, _P> &with, Matrix<T, M, _P> *out) const {
    if (N != _N)
      throw;

    for (size_t i = 0; i < M; i++)
      for (size_t j = 0; j < _P; j++) {
        (*out)[i][j] = 0;
        for (size_t k = 0; k < N; k++)
          (*out)[i][j] += matrix[i][k] * with.get(k, j);
      }
  }

public:
  Matrix() = default;

  Matrix(std::initializer_list<std::initializer_list<T>> list) {
    if (M == 0)
      return;
    if (list.size() > M)
      throw std::out_of_range{"Too many rows."};

    size_t i = 0;
    for (auto &row : list) {
      if (row.size() > N)
        throw std::out_of_range{"Too many cols."};

      size_t j = 0;
      for (auto &el : row)
        matrix[i][j++] = el;
      i++;
    }
  }

  Matrix(const T arr[M][N]) {
    T *m = *matrix;
    for (const T *p = *arr; p < *(arr + M - 1) + N; ++p)
      *m++ = *p;
  }

  bool isSquare() const {
    if (M == N)
      return true;
  }

  void fill(T (*f)(size_t, size_t)) {
    for (size_t i = 0; i < M; i++)
      for (size_t j = 0; j < N; j++)
        matrix[i][j] = f(i, j);
  }

  template <size_t _N, size_t _P>

  /* Multiply to Matrices together */
  Matrix<T, M, _P> *mul(const Matrix<T, _N, _P> &mat) const {
    Matrix<T, M, _P> *out = new Matrix<T, M, _P>();
    _multiply(mat, out);

    return out;
  }

  // WARNING: verificicar corretude

  void transpose() {
    for (size_t i = 0; i < N; i++)
      for (size_t j = 0; j < i; j++) {
        T swap = this->matrix[i][j];
        this->matrix[i][j] = this->matrix[j][i];
        this->matrix[j][i] = swap;
      }
  }

  Matrix<T, M, N> *getTranspose() const {
    auto *out = new Matrix<T, N, M>;
    T t[N][M] = (T[N][M])(*out);

    for (size_t i = 0; i < M; i++)
      for (size_t j = 0; j < N; j++)
        t[j][i] = matrix[i][j];

    return out;
  }

  // std::array<T, N>
  T *operator[](const size_t i) { return matrix[i]; }

  T get(const size_t i, const size_t j) const { return *(*(matrix + i) + j); }

  Matrix &operator+=(const Matrix<T, M, N> &mat) {
    for (size_t i = 0; i < M; i++)
      for (size_t j = 0; j < N; j++)
        matrix[i][j] += mat.get(i, j);
    return *this;
  }

  Matrix &operator-=(const Matrix<T, M, N> &mat) {
    for (size_t i = 0; i < M; i++)
      for (size_t j = 0; j < N; j++)
        matrix[i][j] -= mat.get(i, j);
    return *this;
  }

  Matrix &operator*=(const T scalar) {
    for (size_t i = 0; i < M; i++)
      for (size_t j = 0; i < N; j++)
        matrix[i][j] *= scalar;
    return *this;
  }

  Matrix &operator/=(const T scalar) {
    for (size_t i = 0; i < M; i++)
      for (size_t j = 0; i < N; j++)
        matrix[i][j] /= scalar;
    return *this;
  }

  Matrix operator+(const Matrix<T, M, N> &mat) {
    Matrix<T, M, N> new_mat;

    for (size_t i = 0; i < M; i++)
      for (size_t j = 0; j < N; j++)
        new_mat[i][j] = get(i, j) + mat.get(i, j);

    return new_mat;
  }

  Matrix operator-(const Matrix<T, M, N> &mat) {
    Matrix<T, M, N> new_mat;

    for (size_t i = 0; i < M; i++)
      for (size_t j = 0; j < N; j++)
        new_mat[i][j] = get(i, j) - mat.get(i, j);

    return new_mat;
  }

  Matrix &operator+() { return this; }
  Matrix &operator-() {
    for (size_t i = 0; i < M; i++)
      for (size_t j = 0; j < N; j++)
        matrix[i][j] *= -1;
    return this;
  }

  Matrix operator*(T scalar) {
    Matrix<T, M, N> new_mat;

    for (size_t i = 0; i < M; i++)
      for (size_t j = 0; j < N; j++)
        new_mat[i][j] = get(i, j) * scalar;

    return new_mat;
  }

  template <size_t O> Matrix<T, M, O> operator*(const Matrix<T, N, O> &mat) {
    Matrix<T, M, O> new_mat;

    for (size_t i = 0; i < M; i++)
      for (size_t j = 0; j < O; j++) {
        new_mat[i][j] = 0;
        for (size_t k = 0; k < N; k++)
          new_mat[i][j] += get(i, k) * mat.get(k, j);
      }

    return new_mat;
  }

  friend std::ostream &operator<<(std::ostream &out, const Matrix &mat) {
    out << '[' << std::endl;
    for (size_t i = 0; i < M; i++) {
      out << '[';
      if (N > 0)
        out << mat.get(i, 0);
      for (size_t j = 1; j < M; j++)
        out << ", " << mat.get(i, j);
      out << ']' << std::endl;
    }
    out << ']' << std::endl;

    return out;
  }
};

template <class T, size_t N> class SquareMatrix : public Matrix<T, N, N> {
public:
  using Matrix<T, N, N>::Matrix;
  using Matrix<T, N, N>::mul;

  bool isSquare() const { return true; }

  SquareMatrix<T, N> *mul(const SquareMatrix<T, N> &mat) const {
    SquareMatrix<T, N> *out = new SquareMatrix<T, N>();
    this->_multiply(mat, out);

    return out;
  }

  // TODO -> Implement
  void invert();
  T getDeterminant();
  SquareMatrix *getAdjugate();

  // TODO -> Usar algoritmo otimizado
  SquareMatrix *getInverse() {
    T det = getDeterminant();
    if (det == 0)
      throw;
    SquareMatrix *adj = getAdjugate();
    *adj *= 1 / det;

    return adj;
  }
};

template <class T> using Matrix2 = SquareMatrix<T, 2>;
template <class T> using Matrix3 = SquareMatrix<T, 3>;
template <class T> using Matrix4 = SquareMatrix<T, 4>;

template <typename T, size_t M, size_t N>
constexpr Matrix<T, M, N> operator*(T scalar, Matrix<T, M, N> &m) {
  return m * scalar;
}

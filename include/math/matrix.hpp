#pragma once

#include <Kokkos_Core.hpp>
#include "./vector.hpp"

namespace math {

  template <class T, int D>
  struct Matrix {
    T data[D][D];

    KOKKOS_FUNCTION Matrix() {}

    KOKKOS_FUNCTION Matrix(T x) {
      #pragma unroll (D)
      for (int i = 0; i < D; i++) {
        #pragma unroll (D)
        for (int j = 0; j < D; j++) {
          data[i][j] = x;
        }
      }
    }

    static KOKKOS_INLINE_FUNCTION Matrix<T, D> zeros() {
      return Matrix<T, D>(0);
    }

    static KOKKOS_INLINE_FUNCTION Matrix<T, D> repeat(T x) {
      return Matrix<T, D>(x);
    }

    static KOKKOS_INLINE_FUNCTION Matrix<T, D> identity() {
      Matrix<T, D> id(0);
      #pragma unroll (D)
      for (int i = 0; i < D; i++) {
        id.data[i][i] = (T) 1;
      }
      return id;
    }

    KOKKOS_INLINE_FUNCTION T &operator()(int i, int j) {
      return data[i][j];
    }

    KOKKOS_INLINE_FUNCTION T operator()(int i, int j) const {
      return data[i][j];
    }

    KOKKOS_INLINE_FUNCTION Matrix<T, D> operator+(const Matrix<T, D> &o) const {
      Matrix<T, D> mat;
      #pragma unroll (D)
      for (int i = 0; i < D; i++) {
        #pragma unroll (D)
        for (int j = 0; j < D; j++) {
          mat.data[i][j] = data[i][j] + o.data[i][j];
        }
      }
      return mat;
    }

    KOKKOS_INLINE_FUNCTION Matrix<T, D> operator-(const Matrix<T, D> &o) const {
      Matrix<T, D> mat;
      #pragma unroll (D)
      for (int i = 0; i < D; i++) {
        #pragma unroll (D)
        for (int j = 0; j < D; j++) {
          mat.data[i][j] = data[i][j] - o.data[i][j];
        }
      }
      return mat;
    }

    KOKKOS_INLINE_FUNCTION Matrix<T, D> operator*(T x) const {
      Matrix<T, D> mat;
      #pragma unroll (D)
      for (int i = 0; i < D; i++) {
        #pragma unroll (D)
        for (int j = 0; j < D; j++) {
          mat.data[i][j] = data[i][j] * x;
        }
      }
      return mat;
    }

    KOKKOS_INLINE_FUNCTION Vector<T, D> operator*(const Vector<T, D> &v) const {
      Vector<T, D> vec;
      #pragma unroll (D)
      for (int i = 0; i < D; i++) {
        vec[i] = 0.0;
        #pragma unroll(D)
        for (int j = 0; j < D; j++) {
          vec[i] += data[i][j] * v[j];
        }
      }
    }
  };
}
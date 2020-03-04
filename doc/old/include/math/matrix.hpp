#pragma once

#include "vector.hpp"

namespace math {
  template <typename T, int dim>
  struct Matrix {
    Vector<T, dim> data[dim];

    explicit Matrix(T d) {
      for (int i = 0; i < dim; i++) {
        data[i](i) = d;
      }
    }
  };
} // namespace math
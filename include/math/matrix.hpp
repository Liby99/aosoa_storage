#pragma once

#include "vector.hpp"

namespace math {
  template <typename T, int dim>
  struct Matrix {
    Vector<T, dim> data[dim];
  };
} // namespace math
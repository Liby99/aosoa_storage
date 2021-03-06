#pragma once

#include "./vector.hpp"
#include "./matrix.hpp"

namespace math {
  using Vector2f = Vector<float, 2>;
  using Vector2d = Vector<double, 2>;
  using Vector2i = Vector<int, 2>;
  using Vector3f = Vector<float, 3>;
  using Vector3d = Vector<double, 3>;
  using Vector3i = Vector<int, 3>;

  using Matrix2f = Matrix<float, 2>;
  using Matrix2d = Matrix<double, 2>;
  using Matrix3f = Matrix<float, 3>;
  using Matrix3d = Matrix<double, 3>;
} // namespace math
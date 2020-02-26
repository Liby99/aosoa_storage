#pragma once

#include "matrix.hpp"
#include "vector.hpp"

namespace math {
  using Vector2f = Vector<float, 2>;
  using Vector3f = Vector<float, 3>;
  using Vector4f = Vector<float, 4>;

  using Vector2d = Vector<double, 2>;
  using Vector3d = Vector<double, 3>;
  using Vector4d = Vector<double, 4>;

  using Vector2i = Vector<int, 2>;
  using Vector3i = Vector<int, 3>;
  using Vector4i = Vector<int, 4>;

  using Matrix2f = Matrix<float, 2>;
  using Matrix3f = Matrix<float, 3>;
  using Matrix4f = Matrix<float, 4>;

  using Matrix2d = Matrix<double, 2>;
  using Matrix3d = Matrix<double, 3>;
  using Matrix4d = Matrix<double, 4>;
}
#pragma once

#include "math/prelude.hpp"

namespace storage {

  template <typename T>
  struct TypeTransform {
    using Type = T;
  };

  template <typename T, int dim>
  struct TypeTransform<math::Vector<T, dim>> {
    using Type = T[4];
  };

  template <typename T>
  struct TypeTransform<math::Matrix<T, 2>> {
    using Type = T[8];
  };

  template <typename T>
  struct TypeTransform<math::Matrix<T, 3>> {
    using Type = T[12];
  };

  template <typename T>
  struct TypeTransform<math::Matrix<T, 4>> {
    using Type = T[16];
  };

} // namespace storage
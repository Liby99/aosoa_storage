#pragma once

#ifdef STORAGE_ENABLED_CUDA
#define FnFlag __host__ __device__
#else
#define FnFlag
#endif

#include "./vector.hpp"

namespace storage_math {
  using Vector2f = Vector<float, 2>;
  using Vector2d = Vector<double, 2>;
  using Vector3f = Vector<float, 3>;
  using Vector3d = Vector<double, 3>;
} // namespace storage_math
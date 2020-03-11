#pragma once

#ifdef STORAGE_ENABLED_CUDA
#include <nvfunctional>
#else
#include <functional>
#endif

#ifdef STORAGE_ENABLED_CUDA
#define Fn nvstd::function
#define FnFlag __host__ __device__
#else
#define Fn std::function
#define FnFlag
#endif

namespace storage {
  template <int Index, typename T, typename... Types>
  struct ExtractTypeAt {
    using Type = typename ExtractTypeAt<Index - 1, Types...>::Type;
  };

  template <typename T, typename... Types>
  struct ExtractTypeAt<0, T, Types...> {
    using Type = T;
  };
} // namespace storage
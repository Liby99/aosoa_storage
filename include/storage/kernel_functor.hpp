#pragma once

#include "./handle.hpp"
#include "./utils.hpp"

namespace storage {
  template <class AoSoA, class F, typename... Types>
  struct LinearKernel {
    using Handle = LinearHandle<AoSoA, Types...>;

    SliceHolder<AoSoA, Types...> slice_holder;

    F &kernel;

    LinearKernel(const AoSoA &data, F &kernel)
        : slice_holder(data), kernel(kernel) {}

    KOKKOS_INLINE_FUNCTION void operator()(const int i) const {
      Handle handle(slice_holder, i);
      kernel(handle);
    }
  };
} // namespace storage
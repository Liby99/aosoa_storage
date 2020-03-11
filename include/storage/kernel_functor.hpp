#pragma once

#include "./utils.hpp"
#include "./handle.hpp"

namespace storage {
  template <class AoSoA, typename... Types>
  struct LinearKernel {
    using Handle = LinearHandle<AoSoA, Types...>;

    const SliceHolder<AoSoA, Types...> &slice_holder;

    const Fn<void(Handle &)> &kernel;

    LinearKernel(const AoSoA &data, const Fn<void(Handle &)> &kernel)
      : slice_holder(data), kernel(kernel) {}

    KOKKOS_INLINE_FUNCTION void operator() (const int i) const {
      Handle handle(slice_holder, i);
      kernel(handle);
    }
  };
}
#pragma once

#include <cstddef>
#include "./storage.hpp"
#include "./utils.hpp"
#include "./kernel_functor.hpp"

namespace storage {
  template <class ExecutionSpace, class MemorySpace, int BinSize, typename... Types>
  struct FullStorage : public Storage<ExecutionSpace, MemorySpace, BinSize, Types...> {
    using Super = Storage<ExecutionSpace, MemorySpace, BinSize, Types...>;

    using AoSoA = typename Super::AoSoA;
    using Handle = typename Super::Handle;

    // Fields
    AoSoA data;
    std::size_t size;

    // Constructor
    FullStorage() : FullStorage(DEFAULT_CAPACITY) {}

    FullStorage(int capacity) : data("storage", capacity), size(0) {}

    // Iteration
    void par_each(const Fn<void(typename Super::Handle &)> &kernel) {
      LinearKernel<AoSoA, Types...> kernel_functor(data, kernel);
      Kokkos::RangePolicy<ExecutionSpace> linear_policy(0, size);
      Kokkos::parallel_for(linear_policy, kernel_functor, "par_each");
    }

    // Useful constants
    static const int DEFAULT_CAPACITY = 1000;
  };
} // namespace storage
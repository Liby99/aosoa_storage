#pragma once

#include <cstddef>
#include "./storage.hpp"

namespace storage {
  template <class ExecutionSpace, class MemorySpace, int BinSize, typename... Types>
  struct FullStorage : public Storage<ExecutionSpace, MemorySpace, BinSize, Types...> {
    using Super = Storage<ExecutionSpace, MemorySpace, BinSize, Types...>;

    // Fields
    typename Super::AoSoA data;
    std::size_t size;

    // Constructor
    FullStorage() : FullStorage(DEFAULT_CAPACITY) {}

    FullStorage(int capacity) : data("storage", capacity), size(0) {}

    // Useful constants
    static const int DEFAULT_CAPACITY = 1000;
  };
} // namespace storage
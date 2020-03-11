#pragma once

#include "./storage.hpp"

namespace storage {
  template <class ExecutionSpace, class MemorySpace, int BinSize, typename... Types>
  struct FullStorage : public Storage<ExecutionSpace, MemorySpace, BinSize, Types...> {};
} // namespace storage
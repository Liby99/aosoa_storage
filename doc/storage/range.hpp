#pragma once

#include <cstddef>

namespace storage {
  struct Range {
    std::size_t start;
    std::size_t amount;
  };
} // namespace storage
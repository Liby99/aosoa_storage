#pragma once

namespace storage {
  template <typename T>
  struct TypeTransform {
    using From = T;

    using To = T;
  };
} // namespace storage
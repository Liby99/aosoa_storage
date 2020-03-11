#pragma once

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
#pragma once

#include <Cabana_Core.hpp>

namespace storage {
  template <int Index, typename AoSoA>
  struct SliceHolderBase {
    decltype(Cabana::slice<Index>(std::declval<AoSoA>())) slice;

    SliceHolderBase(const AoSoA &data) : slice(Cabana::slice<Index>(data)) {}
  };

  template <int Index, typename AoSoA, typename... Types>
  struct SliceHolderImpl {
    SliceHolderImpl(const AoSoA &data) {}
  };

  template <int Index, typename AoSoA, typename T, typename... Types>
  struct SliceHolderImpl<Index, AoSoA, T, Types...>
      : public SliceHolderBase<Index, AoSoA>,
        public SliceHolderImpl<Index + 1, AoSoA, Types...> {
    SliceHolderImpl(const AoSoA &data)
        : SliceHolderBase<Index, AoSoA>(data),
          SliceHolderImpl<Index + 1, AoSoA, Types...>(data) {}
  };

  template <typename AoSoA, typename... Types>
  struct SliceHolder : public SliceHolderImpl<0, AoSoA, Types...> {
    template <int Index>
    using SliceAt = decltype(Cabana::slice<Index>(std::declval<AoSoA>()));

    SliceHolder(const AoSoA &data) : SliceHolderImpl<0, AoSoA, Types...>(data) {}

    template <int Index>
    KOKKOS_INLINE_FUNCTION const SliceAt<Index> &get() const {
      return SliceHolderBase<Index, AoSoA>::slice;
    }
  };
}
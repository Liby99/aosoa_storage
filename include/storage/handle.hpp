#pragma once

#include "./slice_holder.hpp"
#include "./type_transform.hpp"
#include "./utils.hpp"

namespace storage {
  template <class AoSoA, typename... Types>
  struct LinearHandle {
    template <int Index>
    using TypeAt = typename ExtractTypeAt<Index, Types...>::Type;

    template <int Index>
    using TransformedTypeAt = typename TypeTransform<TypeAt<Index>>::To;

    const SliceHolder<AoSoA, Types...> &slice_holder;

    const int i;

    KOKKOS_FUNCTION LinearHandle(const SliceHolder<AoSoA, Types...> &slice_holder, const int i)
        : slice_holder(slice_holder), i(i) {}

    template <int Index>
    KOKKOS_INLINE_FUNCTION TypeAt<Index> get() const {
      return TypeTransform<TypeAt<Index>>::get(slice_holder.template get<Index>(), i);
    }

    template <int Index>
    KOKKOS_INLINE_FUNCTION void set(const TypeAt<Index> &c) const {
      TypeTransform<TypeAt<Index>>::set(slice_holder.template get<Index>(), i, c);
    }
  };
} // namespace storage
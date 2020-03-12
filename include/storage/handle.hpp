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

  template <template <class> typename Extractor, typename... Storages>
  struct JoinedLinearHandle {
    using Base = JoinedStorageGroup<Storages...>;

    using SliceHolder = JoinedSliceHolder<Extractor, Storages...>;

    using Offset = JoinedOffset<Base, Storages...>;

    template <int StorageIndex>
    using StorageAt = typename ExtractTypeAt<StorageIndex, Storages...>::Type;

    template <int StorageIndex, int FieldIndex>
    using TypeAt = typename StorageAt<StorageIndex>::template TypeAt<FieldIndex>;

    const SliceHolder &slice_holder;

    const Offset &offset;

    const int i;

    KOKKOS_FUNCTION JoinedLinearHandle(const SliceHolder &slice_holder, const Offset &offset, const int i)
        : slice_holder(slice_holder), offset(offset), i(i) {}

    template <int StorageIndex, int FieldIndex>
    KOKKOS_INLINE_FUNCTION TypeAt<StorageIndex, FieldIndex> get() const {
      return TypeTransform<TypeAt<StorageIndex, FieldIndex>>::get(
        slice_holder.template get<StorageIndex, FieldIndex>(),
        offset.template local_offset<StorageIndex>() + i);
    }

    template <int StorageIndex, int FieldIndex>
    KOKKOS_INLINE_FUNCTION void set(const TypeAt<StorageIndex, FieldIndex> &c) const {
      TypeTransform<TypeAt<StorageIndex, FieldIndex>>::set(
        slice_holder.template get<StorageIndex, FieldIndex>(),
        offset.template local_offset<StorageIndex>() + i,
        c);
    }
  };
} // namespace storage
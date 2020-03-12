#pragma once

#include <Cabana_Core.hpp>
#include "./utils.hpp"

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
      : public SliceHolderBase<Index, AoSoA>, public SliceHolderImpl<Index + 1, AoSoA, Types...> {
    SliceHolderImpl(const AoSoA &data)
        : SliceHolderBase<Index, AoSoA>(data), SliceHolderImpl<Index + 1, AoSoA, Types...>(data) {}
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

  template <int StorageIndex, int FieldIndex, template <class> typename Extractor, typename S>
  struct JoinedSliceHolderBaseImpl : public JoinedSliceHolderBaseImpl<StorageIndex, FieldIndex - 1, Extractor, S> {
    using AoSoA = typename Extractor<S>::AoSoA;

    using Slice = decltype(Cabana::slice<FieldIndex>(std::declval<AoSoA>()));

    Slice slice;

    JoinedSliceHolderBaseImpl(const S &s)
        : slice(Cabana::slice<FieldIndex>(Extractor<S>::get(s))),
          JoinedSliceHolderBaseImpl<StorageIndex, FieldIndex - 1, Extractor, S>(s) {}
  };

  template <int StorageIndex, template <class> typename Extractor, typename S>
  struct JoinedSliceHolderBaseImpl<StorageIndex, -1, Extractor, S> {
    JoinedSliceHolderBaseImpl(const S &s) {}
  };

  template <int StorageIndex, template <class> typename Extractor, typename S>
  struct JoinedSliceHolderBase : JoinedSliceHolderBaseImpl<StorageIndex, S::N - 1, Extractor, S> {
    JoinedSliceHolderBase(const S &s) : JoinedSliceHolderBaseImpl<StorageIndex, S::N - 1, Extractor, S>(s) {}
  };

  template <int StorageIndex, template <class> typename Extractor, typename... Storages>
  struct JoinedSliceHolderImpl {
    JoinedSliceHolderImpl(const Storages &... ss) {}
  };

  template <int StorageIndex, template <class> typename Extractor, typename S, typename... Storages>
  struct JoinedSliceHolderImpl<StorageIndex, Extractor, S, Storages...>
    : public JoinedSliceHolderBase<StorageIndex, Extractor, S>,
      public JoinedSliceHolderImpl<StorageIndex + 1, Extractor, Storages...> {
    JoinedSliceHolderImpl(const S &s, const Storages &... ss)
        : JoinedSliceHolderBase<StorageIndex, Extractor, S>(s),
          JoinedSliceHolderImpl<StorageIndex + 1, Extractor, Storages...>(ss...) {}
  };

  template <template <class> typename Extractor, typename... Storages>
  struct JoinedSliceHolder : public JoinedSliceHolderImpl<0, Extractor, Storages...> {
    JoinedSliceHolder(const Storages &... ss) : JoinedSliceHolderImpl<0, Extractor, Storages...>(ss...) {}

    template <int StorageIndex>
    using StorageAt = typename ExtractTypeAt<StorageIndex, Storages...>::Type;

    template <int StorageIndex, int FieldIndex>
    using SliceAt = typename JoinedSliceHolderBaseImpl<StorageIndex, FieldIndex, Extractor, StorageAt<StorageIndex>>::Slice;

    template <int StorageIndex, int FieldIndex>
    KOKKOS_INLINE_FUNCTION const SliceAt<StorageIndex, FieldIndex> &get() const {
      return JoinedSliceHolderBaseImpl<StorageIndex, FieldIndex, Extractor, StorageAt<StorageIndex>>::slice;
    }
  };

  template <typename S>
  struct HostAoSoAExtractor {
    using AoSoA = typename S::HostAoSoA;

    static AoSoA get(const S &s) {
      return s.host_data;
    }
  };

  template <typename S>
  struct DeviceAoSoAExtractor {
    using AoSoA = typename S::DeviceAoSoA;

    static AoSoA get(const S &s) {
      return s.device_data;
    }
  };
} // namespace storage
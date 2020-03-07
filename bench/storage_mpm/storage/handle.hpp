#pragma once

#include "../../config.hpp"

#include "./utils.hpp"
#include "./type_transform.hpp"

template <int Index, typename AoSoA>
struct SliceHolderBase {
  decltype(Cabana::slice<Index>(std::declval<AoSoA>())) slice;

  SliceHolderBase(AoSoA &aosoa) : slice(Cabana::slice<Index>(aosoa)) {}
};

template <int Index, typename AoSoA, typename... Types>
struct SliceHolder {
  SliceHolder(AoSoA &aosoa) {}
};

template <int Index, typename AoSoA, typename T, typename... Types>
struct SliceHolder<Index, AoSoA, T, Types...> : public SliceHolderBase<Index, AoSoA>,
                                                public SliceHolder<Index + 1, AoSoA, Types...> {
  SliceHolder(AoSoA &aosoa)
      : SliceHolderBase<Index, AoSoA>(aosoa), SliceHolder<Index + 1, AoSoA, Types...>(aosoa) {}
};

template <typename... Types>
struct ElementHandle {
  using MemberTypes = Cabana::MemberTypes<typename TypeTransform<Types>::To...>;

  using CabanaAoSoA = Cabana::AoSoA<MemberTypes, KokkosDevice, BIN_SIZE>;

  template <int Index>
  using TypeAt = typename ExtractTypeAt<Index, Types...>::Type;

  const SliceHolder<0, CabanaAoSoA, Types...> &slice_holder;

  int i;

  ElementHandle(const SliceHolder<0, CabanaAoSoA, Types...> &slice_holder, int i)
      : slice_holder(slice_holder), i(i) {}

  template <int Index>
  TypeAt<Index> fetch() const {
    auto slice = static_cast<SliceHolderBase<Index, CabanaAoSoA>>(slice_holder).slice;
    return TypeTransform<TypeAt<Index>>::fetch(slice, i);
  }

  template <int Index>
  void store(const TypeAt<Index> &comp) {
    auto slice = static_cast<SliceHolderBase<Index, CabanaAoSoA>>(slice_holder).slice;
    TypeTransform<TypeAt<Index>>::store(slice, i, comp);
  }
};

template <typename... Types>
struct SimdElementHandle {
  using MemberTypes = Cabana::MemberTypes<typename TypeTransform<Types>::To...>;

  using CabanaAoSoA = Cabana::AoSoA<MemberTypes, KokkosDevice, BIN_SIZE>;

  template <int Index>
  using TypeAt = typename ExtractTypeAt<Index, Types...>::Type;

  const SliceHolder<0, CabanaAoSoA, Types...> &slice_holder;

  int s, a;

  SimdElementHandle(const SliceHolder<0, CabanaAoSoA, Types...> &slice_holder, int s, int a)
      : slice_holder(slice_holder), s(s), a(a) {}

  template <int Index>
  TypeAt<Index> fetch() const {
    auto slice = static_cast<SliceHolderBase<Index, CabanaAoSoA>>(slice_holder).slice;
    return TypeTransform<TypeAt<Index>>::fetch(slice, s, a);
  }

  template <int Index>
  void store(const TypeAt<Index> &comp) {
    auto slice = static_cast<SliceHolderBase<Index, CabanaAoSoA>>(slice_holder).slice;
    TypeTransform<TypeAt<Index>>::store(slice, s, a, comp);
  }
};
#pragma once

#include "../../../common/config.hpp"

#include "./type_transform.hpp"
#include "./utils.hpp"

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
struct SliceHolder<Index, AoSoA, T, Types...>
    : public SliceHolderBase<Index, AoSoA>,
      public SliceHolder<Index + 1, AoSoA, Types...> {
  SliceHolder(AoSoA &aosoa)
      : SliceHolderBase<Index, AoSoA>(aosoa),
        SliceHolder<Index + 1, AoSoA, Types...>(aosoa) {}
};

template <typename... Types>
struct ElementHandle {
  using MemberTypes = Cabana::MemberTypes<typename TypeTransform<Types>::To...>;

  using CabanaAoSoA = Cabana::AoSoA<MemberTypes, KokkosDevice, BIN_SIZE>;

  template <int Index>
  using TypeAt = typename ExtractTypeAt<Index, Types...>::Type;

  template <int Index>
  using SliceAt = decltype(Cabana::slice<Index>(std::declval<CabanaAoSoA>()));

  const SliceHolder<0, CabanaAoSoA, Types...> &slice_holder;

  int i;

  ElementHandle(const SliceHolder<0, CabanaAoSoA, Types...> &slice_holder,
                int i)
      : slice_holder(slice_holder), i(i) {}

  template <int Index>
  inline const SliceAt<Index> &slice() const {
    return ((const SliceHolderBase<Index, CabanaAoSoA> &)slice_holder).slice;
  }

  template <int Index>
  inline TypeAt<Index> fetch() const {
    const auto &slice =
        static_cast<SliceHolderBase<Index, CabanaAoSoA>>(slice_holder).slice;
    return TypeTransform<TypeAt<Index>>::fetch(slice, i);
  }

  template <int Index>
  inline void store(const TypeAt<Index> &comp) {
    const auto &slice =
        static_cast<SliceHolderBase<Index, CabanaAoSoA>>(slice_holder).slice;
    TypeTransform<TypeAt<Index>>::store(slice, i, comp);
  }
};

template <typename... Types>
struct SimdElementHandle {
  using MemberTypes = Cabana::MemberTypes<typename TypeTransform<Types>::To...>;

  using CabanaAoSoA = Cabana::AoSoA<MemberTypes, KokkosDevice, BIN_SIZE>;

  template <int Index>
  using TypeAt = typename ExtractTypeAt<Index, Types...>::Type;

  template <int Index>
  using SliceAt = decltype(Cabana::slice<Index>(std::declval<CabanaAoSoA>()));

  const SliceHolder<0, CabanaAoSoA, Types...> &slice_holder;

  int s, a;

  SimdElementHandle(const SliceHolder<0, CabanaAoSoA, Types...> &slice_holder,
                    int s,
                    int a)
      : slice_holder(slice_holder), s(s), a(a) {}

  template <int Index>
  inline const SliceAt<Index> &slice() const {
    return ((const SliceHolderBase<Index, CabanaAoSoA> &)slice_holder).slice;
  }

  template <int Index>
  inline TypeAt<Index> fetch() const {
    return TypeTransform<TypeAt<Index>>::fetch(slice<Index>(), s, a);
  }

  template <int Index>
  inline void store(const TypeAt<Index> &comp) {
    TypeTransform<TypeAt<Index>>::store(slice<Index>(), s, a, comp);
  }
};
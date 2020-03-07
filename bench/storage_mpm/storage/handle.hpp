#pragma once

#include "../../config.hpp"

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
struct SliceHolder<Index, AoSoA, T, Types...> :
  public SliceHolderBase<Index, AoSoA>,
  public SliceHolder<Index + 1, AoSoA, Types...> {
  SliceHolder(AoSoA &aosoa)
    : SliceHolderBase<Index, AoSoA>(aosoa),
      SliceHolder<Index + 1, AoSoA, Types...>(aosoa) {}
};

template <typename... Types>
struct ElementHandle :
  public SliceHolder<
    0,
    Cabana::AoSoA<
      Cabana::MemberTypes<
        typename TypeTransform<Types>::To ...
      >,
      KokkosDevice,
      BIN_SIZE
    >,
    Types...
  >
{
  using MemberTypes = Cabana::MemberTypes<typename TypeTransform<Types>::To ...>;

  using CabanaAoSoA = Cabana::AoSoA<MemberTypes, KokkosDevice, BIN_SIZE>;

  template <int Index>
  using TypeAt = typename ExtractTypeAt<Index, Types...>::Type;

  int index;

  ElementHandle(CabanaAoSoA &data, int index) : SliceHolder<0, CabanaAoSoA, Types...>(data), index(index) {}

  template <int Index>
  TypeAt<Index> fetch() const {
    return TypeTransform<TypeAt<Index>>::fetch(SliceHolderBase<Index, CabanaAoSoA>::slice, index);
  }

  template <int Index>
  void store(const TypeAt<Index> &comp) {
    TypeTransform<TypeAt<Index>>::store(SliceHolderBase<Index, CabanaAoSoA>::slice, index, comp);
  }
};

template <typename... Types>
struct SimdElementHandle : public SliceHolder<
    0,
    Cabana::AoSoA<
      Cabana::MemberTypes<
        typename TypeTransform<Types>::To ...
      >,
      KokkosDevice,
      BIN_SIZE
    >,
    Types...
  > {
  using MemberTypes = Cabana::MemberTypes<typename TypeTransform<Types>::To ...>;

  using CabanaAoSoA = Cabana::AoSoA<MemberTypes, KokkosDevice, BIN_SIZE>;

  template <int Index>
  using TypeAt = typename ExtractTypeAt<Index, Types...>::Type;

  int s, a;

  SimdElementHandle(CabanaAoSoA &data, int s, int a) : SliceHolder<0, CabanaAoSoA, Types...>(data), s(s), a(a) {}

  template <int Index>
  TypeAt<Index> fetch() const {
    return TypeTransform<TypeAt<Index>>::fetch(SliceHolderBase<Index, CabanaAoSoA>::slice, s, a);
  }

  template <int Index>
  void store(const TypeAt<Index> &comp) {
    TypeTransform<TypeAt<Index>>::store(SliceHolderBase<Index, CabanaAoSoA>::slice, s, a, comp);
  }
};
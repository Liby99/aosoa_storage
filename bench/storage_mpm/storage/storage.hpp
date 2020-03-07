#pragma once

#include "../../config.hpp"

#include "../utils/range.hpp"
#include "../math/vector.hpp"

#include "./type_transform.hpp"
#include "./utils.hpp"

template <typename... Types>
struct ElementHandle {
  using MemberTypes = Cabana::MemberTypes<typename TypeTransform<Types>::To ...>;

  using CabanaAoSoA = Cabana::AoSoA<MemberTypes, KokkosDevice, BIN_SIZE>;

  template <int Index>
  using TypeAt = typename ExtractTypeAt<Index, Types...>::Type;

  CabanaAoSoA &data;

  int index;

  ElementHandle(CabanaAoSoA &data, int index) : data(data), index(index) {}

  template <int Index>
  TypeAt<Index> fetch() const {
    auto slice = Cabana::slice<Index>(data);
    return TypeTransform<TypeAt<Index>>::fetch(slice, index);
  }

  template <int Index>
  void store(const TypeAt<Index> &comp) {
    auto slice = Cabana::slice<Index>(data);
    TypeTransform<TypeAt<Index>>::store(slice, index, comp);
  }
};

template <typename... Types>
struct SimdElementHandle {
  using MemberTypes = Cabana::MemberTypes<typename TypeTransform<Types>::To ...>;

  using CabanaAoSoA = Cabana::AoSoA<MemberTypes, KokkosDevice, BIN_SIZE>;

  template <int Index>
  using TypeAt = typename ExtractTypeAt<Index, Types...>::Type;

  CabanaAoSoA &data;

  int s, a;

  SimdElementHandle(CabanaAoSoA &data, int s, int a) : data(data), s(s), a(a) {}

  template <int Index>
  TypeAt<Index> fetch() const {
    auto slice = Cabana::slice<Index>(data);
    return TypeTransform<TypeAt<Index>>::fetch(slice, s, a);
  }

  template <int Index>
  void store(const TypeAt<Index> &comp) {
    auto slice = Cabana::slice<Index>(data);
    TypeTransform<TypeAt<Index>>::store(slice, s, a, comp);
  }
};

template <typename... Types>
struct Storage {
  using MemberTypes = Cabana::MemberTypes<typename TypeTransform<Types>::To ...>;

  using CabanaAoSoA = Cabana::AoSoA<MemberTypes, KokkosDevice, BIN_SIZE>;

  CabanaAoSoA data;

  int size;

  Storage(int capacity) : data("storage", capacity), size(0) {}

  template <typename Iter, typename F>
  Range insert_iter(Iter iter, F callback) {
    ElementHandle<Types...> handle(data, size);
    int start = size;
    for (auto data : iter) {
      handle.index = size++;
      callback(data, handle);
    }
    int length = size - start;
    return Range { start, length };
  }

  template <typename F>
  void each(F callback) {
    ElementHandle<Types...> handle(data, 0);
    for (int i = 0; i < size; i++) {
      handle.index = i;
      callback(handle);
    }
  }

  template <typename F>
  void par_each(F callback) {
    auto kernel = KOKKOS_LAMBDA(const int i) {
      ElementHandle<Types...> handle(data, i);
      callback(handle);
    };
    Kokkos::RangePolicy<ExecutionSpace> linear_policy(0, size);
    Kokkos::parallel_for(linear_policy, kernel, "par_each");
  }

  template <typename F>
  void simd_par_each(F callback) {
    auto kernel = KOKKOS_LAMBDA(const int s, const int a) {
      SimdElementHandle<Types...> handle(data, s, a);
      callback(handle);
    };
    Cabana::SimdPolicy<BIN_SIZE, ExecutionSpace> simd_policy(0, size);
    Cabana::simd_parallel_for(simd_policy, kernel, "simd_par_each");
  }
};
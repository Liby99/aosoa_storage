#pragma once

#include "../../config.hpp"

#include "../utils/range.hpp"
#include "../math/vector.hpp"

#include "./type_transform.hpp"
#include "./handle.hpp"
#include "./utils.hpp"

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
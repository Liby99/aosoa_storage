#pragma once

#include "../../../common/config.hpp"

#include "../math/vector.hpp"
#include "../utils/range.hpp"

#include "./handle.hpp"
#include "./type_transform.hpp"
#include "./utils.hpp"

#ifdef STORAGE_ENABLED_CUDA
#include <nvfunctional>
#define Fn nvstd::function
#else
#include <functional>
#define Fn std::function
#endif

template <class AoSoA, typename... Types>
struct ParEachFunctor {
  const SliceHolder<0, AoSoA, Types...> &slice_holder;
  const Fn<void(ElementHandle<Types...> &)> &callback;

  ParEachFunctor(
    const AoSoA &data,
    const Fn<void(ElementHandle<Types...> &)> &callback
  ) : slice_holder(data), callback(callback) {}

  KOKKOS_INLINE_FUNCTION void operator() (const int i) const {
    ElementHandle<Types...> handle(slice_holder, i);
    callback(handle);
  }
};

template <typename... Types>
struct Storage {
  using MemberTypes = Cabana::MemberTypes<typename TypeTransform<Types>::To...>;

  using CabanaAoSoA = Cabana::AoSoA<MemberTypes, KokkosDevice, BIN_SIZE>;

  template <int Index>
  using TypeAt = typename ExtractTypeAt<Index, Types...>::Type;

  template <int Index>
  using SliceAt = decltype(Cabana::slice<Index>(std::declval<CabanaAoSoA>()));

  using Handle = ElementHandle<Types...>;

  using SimdHandle = SimdElementHandle<Types...>;

  CabanaAoSoA data;

  int size;

  Storage(int capacity) : data("storage", capacity), size(0) {}

  template <typename Iter, typename F>
  Range insert_iter(Iter iter, F callback) {
    SliceHolder<0, CabanaAoSoA, Types...> slice_holder(data);
    int start = size;
    for (auto data : iter) {
      Handle handle(slice_holder, size++);
      callback(data, handle);
    }
    int length = size - start;
    return Range{start, length};
  }

  template <int Index>
  SliceAt<Index> slice() {
    return Cabana::slice<Index>(data);
  }

  template <typename F>
  void each(F callback) const {
    SliceHolder<0, CabanaAoSoA, Types...> slice_holder(data);
    for (int i = 0; i < size; i++) {
      Handle handle(slice_holder, i);
      callback(handle);
    }
  }

  void par_each(const Fn<void(Handle &)> &callback) const {
    ParEachFunctor<CabanaAoSoA, Types...> kernel(data, callback);
    Kokkos::RangePolicy<ExecutionSpace> linear_policy(0, size);
    Kokkos::parallel_for(linear_policy, kernel, "par_each");
  }

  // void simd_par_each(Fn<void(SimdHandle &)> callback) const {
  //   SliceHolder<0, CabanaAoSoA, Types...> slice_holder(data);
  //   auto kernel = KOKKOS_LAMBDA(const int s, const int a) {
  //     SimdHandle handle(slice_holder, s, a);
  //     callback(handle);
  //   };
  //   Cabana::SimdPolicy<BIN_SIZE, ExecutionSpace> simd_policy(0, size);
  //   Cabana::simd_parallel_for(simd_policy, kernel, "simd_par_each");
  // }
};
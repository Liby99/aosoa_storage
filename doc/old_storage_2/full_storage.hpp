#pragma once

#include "./kernel_functor.hpp"
#include "./range.hpp"
#include "./storage.hpp"
#include "./utils.hpp"
#include <cstddef>

namespace storage {
  template <class ExecutionSpace, class MemorySpace, int BinSize, typename... Types>
  struct FullStorage : public Storage<ExecutionSpace, MemorySpace, BinSize, Types...> {
    using Device = Kokkos::Device<ExecutionSpace, MemorySpace>;

    using MemberTypes = Cabana::MemberTypes<typename TypeTransform<Types>::To...>;

    using AoSoA = Cabana::AoSoA<MemberTypes, Device, BinSize>;

    using HostAoSoA = typename AoSoA::host_mirror_type;

    using Tuple = Cabana::Tuple<MemberTypes>;

    template <int Index>
    using TypeAt = typename ExtractTypeAt<Index, Types...>::Type;

    template <int Index>
    using TransformedTypeAt = typename TypeTransform<TypeAt<Index>>::To;

    using Handle = LinearHandle<AoSoA, Types...>;

    using HostHandle = LinearHandle<HostAoSoA, Types...>;

    // Fields
    AoSoA data;
    HostAoSoA mirror;
    std::size_t size;

    // Constructor
    FullStorage() : FullStorage(DEFAULT_CAPACITY) {}

    FullStorage(std::size_t capacity) :
      data("storage", capacity),
      mirror(Cabana::create_mirror_view(Kokkos::HostSpace(), data)),
      size(0) {}

    // Insertion
    Range fill(std::size_t amount, Types... components) {
      std::size_t start = size;
      size += amount;

      // Allocate enough memory
      if (size > mirror.size()) {
        mirror.resize(size);
      }

      // Get the element to set
      auto tuple = ToCabanaTuple<Types...>::to_cabana(components...);

      // Parallel for
      auto kernel = KOKKOS_LAMBDA(int i) { mirror.setTuple(i, tuple); };
      Kokkos::RangePolicy<ExecutionSpace> linear_policy(start, size);
      Kokkos::parallel_for(linear_policy, kernel, "fill");

      // Update the size
      return Range{start, amount};
    }

    // Host/Device operation
    void push() {
      if (data.size() < mirror.size()) {
        data.reserve(mirror.capacity());
      }
      Cabana::deep_copy(mirror, data);
    }

    void pull() {
      Cabana::deep_copy(data, mirror);
    }

    // Get
    template <int Index>
    TypeAt<Index> get(const int i) {
      auto slice = Cabana::slice<Index>(mirror);
      return TypeTransform<TypeAt<Index>>::get(slice, i);
    }

    // Set
    template <int Index>
    void set(const int i, const TypeAt<Index> &c) {
      auto slice = Cabana::slice<Index>(mirror);
      TypeTransform<TypeAt<Index>>::set(slice, i, c);
    }

    // Iteration
    template <class F>
    void each(const F &kernel) {
      SliceHolder<HostAoSoA, Types...> slice_holder(mirror);
      for (int i = 0; i < size; i++) {
        LinearHandle<HostAoSoA, Types...> handle(slice_holder, i);
        kernel(handle);
      }
    }

    template <class F>
    void par_each(const F &kernel) {
      LinearKernel<AoSoA, Types...> kernel_functor(data, kernel);
      Kokkos::RangePolicy<ExecutionSpace> linear_policy(0, size);
      Kokkos::parallel_for(linear_policy, kernel_functor, "par_each");
    }

    // Useful constants
    static const int DEFAULT_CAPACITY = 1024;
  };
} // namespace storage
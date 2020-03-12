#pragma once

#include "./utils.hpp"
#include "./range.hpp"
#include "./slice_holder.hpp"
#include "./handle.hpp"
#include "./kernel_functor.hpp"

namespace storage {
  template <class Config, typename... Types>
  struct FullStorage {
    using DeviceExecutionSpace = typename Config::DeviceExecutionSpace;

    using HostExecutionSpace = typename Config::HostExecutionSpace;

    using DeviceMemorySpace = typename Config::DeviceMemorySpace;

    using Device = Kokkos::Device<DeviceExecutionSpace, DeviceMemorySpace>;

    using Fields = Cabana::MemberTypes<typename TypeTransform<Types>::To ...>;

    using DeviceAoSoA = Cabana::AoSoA<Fields, Device>;

    using HostAoSoA = decltype(Cabana::create_mirror_view(Kokkos::HostSpace(), std::declval<DeviceAoSoA>()));

    using Tuple = Cabana::Tuple<Fields>;

    using HostHandle = LinearHandle<HostAoSoA, Types...>;

    using DeviceHandle = LinearHandle<DeviceAoSoA, Types...>;

    template <int Index>
    using TypeAt = typename ExtractTypeAt<Index, Types...>::Type;

    std::size_t stored_length;

    DeviceAoSoA device_data;

    HostAoSoA host_data;

    static const std::size_t DEFAULT_CAPACITY = 1024;

    FullStorage() : FullStorage(DEFAULT_CAPACITY) {}

    FullStorage(std::size_t capacity) :
      stored_length(0),
      device_data("full_storage", capacity),
      host_data(Cabana::create_mirror_view(Kokkos::HostSpace(), device_data)) {}

    void push() {
      if (device_data.size() < host_data.size()) {
        device_data.resize(host_data.size());
      }
      Cabana::deep_copy(device_data, host_data);
    }

    void pull() {
      Cabana::deep_copy(host_data, device_data);
    }

    std::size_t size() const {
      return stored_length;
    }

    void fill(const Types &... cs) {
      auto tuple = ToCabanaTuple<Types...>::to_cabana(cs...);
      auto kernel = KOKKOS_LAMBDA(int i) { host_data.setTuple(i, tuple); };
      Kokkos::RangePolicy<HostExecutionSpace> linear_policy(0, stored_length);
      Kokkos::parallel_for(linear_policy, kernel, "fill");
    }

    template <int Index>
    void fill(const TypeAt<Index> &c) {
      auto slice = Cabana::slice<Index>(host_data);
      auto kernel = KOKKOS_LAMBDA(int i) {
        TypeTransform<TypeAt<Index>>::set(slice, i, c);
      };
      Kokkos::RangePolicy<HostExecutionSpace> linear_policy(0, stored_length);
      Kokkos::parallel_for(linear_policy, kernel, "fill");
    }

    Range fill(std::size_t amount, const Types &... cs) {
      std::size_t start = stored_length;

      stored_length += amount;
      if (stored_length > host_data.size()) {
        host_data.resize(stored_length);
      }

      auto tuple = ToCabanaTuple<Types...>::to_cabana(cs...);
      auto kernel = KOKKOS_LAMBDA(int i) { host_data.setTuple(i, tuple); };
      Kokkos::RangePolicy<HostExecutionSpace> linear_policy(start, stored_length);
      Kokkos::parallel_for(linear_policy, kernel, "fill");

      return Range{start, amount};
    }

    template <typename Iter, typename F>
    Range fill_with_iter(Iter iter, F callback) {
      std::size_t start = stored_length;

      std::size_t amount = iter.size();
      stored_length += amount;
      if (stored_length > host_data.size()) {
        host_data.resize(stored_length);
      }

      SliceHolder<HostAoSoA, Types...> slice_holder(host_data);
      for (auto data : iter) {
        LinearHandle<HostAoSoA, Types...> handle(slice_holder, start++);
        callback(data, handle);
      }
      return Range{start, stored_length};
    }

    template <typename F>
    void each(F kernel) const {
      SliceHolder<HostAoSoA, Types...> slice_holder(host_data);
      for (int i = 0; i < stored_length; i++) {
        LinearHandle<HostAoSoA, Types...> handle(slice_holder, i);
        kernel(handle);
      }
    }

    template <typename F>
    void par_each(F kernel) {
      LinearKernel<DeviceAoSoA, F, Types...> kernel_functor(device_data, kernel);
      Kokkos::RangePolicy<DeviceExecutionSpace> linear_policy(0, stored_length);
      Kokkos::parallel_for(linear_policy, kernel_functor, "par_each");
    }
  };
}
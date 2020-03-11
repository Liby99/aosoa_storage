#pragma once

#include "./utils.hpp"
#include <memory>

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

    template <int Index>
    using TypeAt = typename ExtractTypeAt<Index, Types...>::Type;

    std::size_t stored_length;

    DeviceAoSoA device_data;

    HostAoSoA host_data;

    static const int DEFAULT_CAPACITY = 1024;

    FullStorage() : FullStorage(DEFAULT_CAPACITY) {}

    FullStorage(std::size_t capacity) :
      stored_length(0),
      device_data("storage", capacity),
      host_data(Cabana::create_mirror_view(Kokkos::HostSpace(), device_data)) {}

    // Data synchronization
    void push() {
      Cabana::deep_copy(host_data, device_data);
    }

    void pull() {
      Cabana::deep_copy(device_data, host_data);
    }
  };
}
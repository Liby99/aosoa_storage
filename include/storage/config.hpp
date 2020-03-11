#pragma once

namespace storage {
  struct DefaultConfig {
    using DeviceExecutionSpace = Kokkos::Serial;

    using HostExecutionSpace = Kokkos::Serial;

    using DeviceMemorySpace = Kokkos::HostSpace;
  };
}
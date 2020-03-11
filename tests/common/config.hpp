#pragma once

#include <Cabana_Core.hpp>

struct Config {
#ifdef STORAGE_ENABLED_CUDA

  using DeviceExecutionSpace = Kokkos::Cuda;
  using DeviceMemorySpace = Kokkos::CudaSpace;

#else

  using DeviceMemorySpace = Kokkos::HostSpace;

#ifdef STORAGE_ENABLED_OPENMP
  using DeviceExecutionSpace = Kokkos::OpenMP;
#else
  using DeviceExecutionSpace = Kokkos::Serial;
#endif // STORAGE_ENABLED_OPENMP

#endif // STORAGE_ENABLED_CUDA

#ifdef STORAGE_ENABLED_OPENMP
  using HostExecutionSpace = Kokkos::OpenMP;
#else
  using HostExecutionSpace = Kokkos::Serial;
#endif // STORAGE_ENABLED_OPENMP

};
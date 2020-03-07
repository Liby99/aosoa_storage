#pragma once

// This benchmark contains special config because this codebase cannot
// comply to Cuda

#include <Cabana_Core.hpp>

using MemorySpace = Kokkos::HostSpace;
static const std::size_t BIN_SIZE = 4;

#ifdef STORAGE_ENABLED_OPENMP
using ExecutionSpace = Kokkos::OpenMP;
#else
using ExecutionSpace = Kokkos::Serial;
#endif // STORAGE_ENABLED_OPENMP

// Device type
using KokkosDevice = Kokkos::Device<ExecutionSpace, MemorySpace>;
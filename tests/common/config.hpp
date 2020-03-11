#pragma once

#include <Cabana_Core.hpp>

// Execution space
#ifdef STORAGE_ENABLED_CUDA

using ExecutionSpace = Kokkos::Cuda;
using MemorySpace = Kokkos::CudaUVMSpace;
static const std::size_t BIN_SIZE = 32;

#else

using MemorySpace = Kokkos::HostSpace;
static const std::size_t BIN_SIZE = 4;

#ifdef STORAGE_ENABLED_OPENMP
using ExecutionSpace = Kokkos::OpenMP;
#else
using ExecutionSpace = Kokkos::Serial;
#endif // STORAGE_ENABLED_OPENMP

#endif // STORAGE_ENABLED_CUDA
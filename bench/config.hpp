#include <Cabana_Core.hpp>

// Execution space
#if defined STORAGE_ENABLED_CUDA
using ExecutionSpace = Kokkos::Cuda;
#elif defined STORAGE_ENABLED_OPENMP
using ExecutionSpace = Kokkos::OpenMP;
#else
using ExecutionSpace = Kokkos::Serial;
#endif

// Memory space & Bin Size
#if defined STORAGE_ENABLE_CUDA
using MemorySpace = Kokkos::CudaSpace;

static const std::size_t BIN_SIZE = 32;
#else
using MemorySpace = Kokkos::HostSpace;

static const std::size_t BIN_SIZE = 4;
#endif

// Device type
using KokkosDevice = Kokkos::Device<ExecutionSpace, MemorySpace>;
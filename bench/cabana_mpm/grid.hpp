#include <Kokkos_UnorderedMap.hpp>

#include "../config.hpp"

template <class T, int D>
struct Grid {
  Kokkos::View<T * [D + 1][64], KokkosDevice> data;
  Kokkos::View<uint64_t *, KokkosDevice> offsets;
  Kokkos::UnorderedMap<uint64_t, size_t, KokkosDevice> mapping;

  Grid() : data("grid_data", 100), offsets("grid_offsets", 100), mapping() {}
};
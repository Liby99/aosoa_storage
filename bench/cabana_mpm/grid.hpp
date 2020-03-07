#include <Kokkos_UnorderedMap.hpp>

#include "../config.hpp"

template <class T, int D>
struct Grid {
  Kokkos::View<T * [D + 1][64], KokkosDevice> data;
  Kokkos::View<uint64_t *, KokkosDevice> offsets;
  Kokkos::UnorderedMap<uint64_t, size_t, KokkosDevice> mapping;

  Grid() : data("grid_data", 100), offsets("grid_offsets", 100), mapping() {}

  // uint64_t get_zorder(int x, int y) {
  //   uint64_t xmask = 0x5555555555555555UL;
  //   uint64_t ymask = 0xaaaaaaaaaaaaaaaaUL;
  //   return _pdep_u64((uint64_t)x, xmask) | _pdep_u64((uint64_t)y, ymask);
  // }

  // uint64_t get_zorder(int x, int y, int z) {
  //   uint64_t xmask = 0x9249249249249249UL;
  //   uint64_t ymask = 0x2492492492492492UL;
  //   uint64_t zmask = 0x4924924924924924UL;
  //   return _pdep_u64((uint64_t)x, xmask) | _pdep_u64((uint64_t)y, ymask) | _pdep_u64((uint64_t)z, zmask);
  // }
};
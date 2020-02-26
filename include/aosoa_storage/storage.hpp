#pragma once

#include <Cabana_Core.hpp>

namespace AosoaStorage {
  template<class ExSpace, class MemSpace, std::size_t BinSize, typename... Types>
  struct Storage {
    using CabanaDataTypes = Cabana::MemberTypes<Types...>;

    using KokkosDeviceType = Kokkos::Device<ExSpace, MemSpace>;
  };
}
#pragma once

#include "./type_transform.hpp"
#include "./utils.hpp"
#include <Cabana_Core.hpp>

namespace storage {
  template <class ExecutionSpace, class MemorySpace, int BinSize, typename... Types>
  struct Storage {
    using Device = Kokkos::Device<ExecutionSpace, MemorySpace>;

    using MemberTypes = Cabana::MemberTypes<typename TypeTransform<Types>::To...>;

    using AoSoA = Cabana::AoSoA<MemberTypes, Device, BinSize>;

    template <int Index>
    using TypeAt = typename ExtractTypeAt<Index, Types...>::Type;

    template <int Index>
    using TransformedTypeAt = typename TypeTransform<TypeAt<Index>>::To;
  };
} // namespace storage
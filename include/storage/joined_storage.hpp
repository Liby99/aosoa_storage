#pragma once

#include "./range.hpp"
#include "./utils.hpp"

namespace storage {
  template <typename Config, typename... Storages>
  struct JoinedStorage {
    using DeviceExecutionSpace = typename Config::DeviceExecutionSpace;

    using Base = JoinedStorageGroup<Storages...>;

    using HostSliceHolder = JoinedSliceHolder<HostAoSoAExtractor, Base, Storages...>;

    using DeviceSliceHolder = JoinedSliceHolder<DeviceAoSoAExtractor, Base, Storages...>;

    using Offset = JoinedOffset<Base, Storages...>;

    using HostHandle = JoinedLinearHandle<HostAoSoAExtractor, Storages...>;

    using DeviceHandle = JoinedLinearHandle<DeviceAoSoAExtractor, Storages...>;

    static const std::size_t N = sizeof...(Storages);

    Base base;

    JoinedStorage(const Storages &... storages) : base(storages...) {}

    Ranges ranges() const {
      return base.ranges();
    }

    template <typename F>
    void each(F kernel) {
      HostSliceHolder host_slice_holder(base);
      auto global_ranges = ranges();
      for (auto &range : global_ranges) {
        Offset offset(range, base);
        for (int i = 0; i < range.amount; i++) {
          HostHandle handle(host_slice_holder, offset, i);
          kernel(handle);
        }
      }
    }

    template <typename F>
    void par_each(F kernel) {
      DeviceSliceHolder device_slice_holder(base);
      auto global_ranges = ranges();
      for (auto &range : global_ranges) {
        Offset offset(range, base);
        JoinedLinearKernel<DeviceAoSoAExtractor, F, Storages...> kernel_functor(device_slice_holder, offset, kernel);
        Kokkos::RangePolicy<DeviceExecutionSpace> linear_policy(0, range.amount);
        Kokkos::parallel_for(linear_policy, kernel_functor, "par_each");
      }
    }
  };
}
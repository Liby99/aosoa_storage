#pragma once

#include "./range.hpp"
#include "./utils.hpp"

namespace storage {
  template <typename Config, typename... Storages>
  struct JoinedStorage {
    using Base = JoinedStorageGroup<Storages...>;

    using HostSliceHolder = JoinedSliceHolder<HostAoSoAExtractor, Storages...>;

    using DeviceSliceHolder = JoinedSliceHolder<DeviceAoSoAExtractor, Storages...>;

    using Offset = JoinedOffset<Base, Storages...>;

    using HostHandle = JoinedLinearHandle<HostAoSoAExtractor, Storages...>;

    using DeviceHandle = JoinedLinearHandle<DeviceAoSoAExtractor, Storages...>;

    Base base;

    HostSliceHolder host_slice_holder;

    DeviceSliceHolder device_slice_holder;

    JoinedStorage(const Storages &... storages)
        : base(storages...),
          host_slice_holder(storages...),
          device_slice_holder(storages...) {}

    Ranges ranges() const {
      return base.ranges();
    }

    template <typename F>
    void each(F kernel) {
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
      auto global_ranges = ranges();
      for (auto &range : global_ranges) {
        Offset offset(range, base);

      }
    }
  };
}
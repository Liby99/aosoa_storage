#pragma once

namespace storage {
  template <typename Config, typename... Storages>
  struct JoinedStorage {
    JoinedStorage(const Storages &... storages) {}

    template <typename F>
    void each(F kernel) {}

    template <typename F>
    void par_each(F kernel) {}
  };
}
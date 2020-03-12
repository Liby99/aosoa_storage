#pragma once

#include "./range.hpp"

namespace storage {
  template <std::size_t Index, typename S>
  struct JoinedStorageBase {
    const S &storage;

    JoinedStorageBase(const S &storage) : storage(storage) {}
  };

  template <std::size_t Index, typename... Storages>
  struct JoinedStorageImpl {
    JoinedStorageImpl(const Storages &... ss) {}

    Ranges ranges() const {
      return Ranges(true); // Infinity
    }
  };

  template <std::size_t Index, typename S, typename... Storages>
  struct JoinedStorageImpl<Index, S, Storages...>
      : public JoinedStorageBase<Index, S>, public JoinedStorageImpl<Index + S::N, Storages...> {
    JoinedStorageImpl(const S &s, const Storages &... ss)
        : JoinedStorageBase<Index, S>(s), JoinedStorageImpl<Index + S::N, Storages...>(ss...) {}

    Ranges ranges() const {
      auto hd = JoinedStorageBase<Index, S>::storage.ranges().globals;
      auto rs = JoinedStorageImpl<Index + S::N, Storages...>::ranges();
      return hd.intersect(rs);
    }
  };

  template <typename Config, typename... Storages>
  struct JoinedStorage {
    JoinedStorageImpl<0, Storages...> impl;

    JoinedStorage(const Storages &... storages) : impl(storages...) {}

    Ranges ranges() const {
      return impl.ranges();
    }

    template <typename F>
    void each(F kernel) {}

    template <typename F>
    void par_each(F kernel) {}
  };
}
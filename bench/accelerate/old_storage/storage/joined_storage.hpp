#pragma once

namespace storage {
  template <std::size_t Index, typename S>
  class JoinedStorageBase {
  public:
    using RefTuple = typename S::RefTuple;

    using ConstRefTuple = typename S::ConstRefTuple;

    S &storage;

    JoinedStorageBase(S &storage) : storage(storage) {}
  };

  template <std::size_t Index, typename... Storages>
  class JoinedStorageGroup {
  public:
    using RefTuple = std::tuple<>;

    using ConstRefTuple = std::tuple<>;

    bool contains(std::size_t i) const {
      return true;
    }

    std::tuple<> get_unchecked(std::size_t i) {
      return std::tie();
    }

    std::tuple<> get_unchecked_const(std::size_t i) const {
      return std::tie();
    }
  };

  template <std::size_t Index, typename S, typename... Storages>
  class JoinedStorageGroup<Index, S, Storages...>
      : public JoinedStorageBase<Index, S>, public JoinedStorageGroup<Index + 1, Storages...> {
  public:
    using RefTuple = decltype(std::tuple_cat(
        std::declval<typename JoinedStorageBase<Index, S>::RefTuple>(),
        std::declval<typename JoinedStorageGroup<Index + 1, Storages...>::RefTuple>()));

    using ConstRefTuple = decltype(std::tuple_cat(
        std::declval<typename JoinedStorageBase<Index, S>::ConstRefTuple>(),
        std::declval<typename JoinedStorageGroup<Index + 1, Storages...>::ConstRefTuple>()));

    JoinedStorageGroup(S &s, Storages &... ss)
        : JoinedStorageBase<Index, S>(s), JoinedStorageGroup<Index + 1, Storages...>(ss...) {}

    bool contains(std::size_t i) const {
      if (JoinedStorageBase<Index, S>::storage.contains(i))
        return JoinedStorageGroup<Index + 1, Storages...>::contains(i);
      return false;
    }

    S &first_storage() const {
      return JoinedStorageBase<Index, S>::storage;
    }

    RefTuple get_unchecked(std::size_t i) {
      return std::tuple_cat(JoinedStorageBase<Index, S>::storage.get_unchecked(i),
                            JoinedStorageGroup<Index + 1, Storages...>::get_unchecked(i));
    }

    ConstRefTuple get_unchecked_const(std::size_t i) const {
      return std::tuple_cat(JoinedStorageBase<Index, S>::storage.get_unchecked_const(i),
                            JoinedStorageGroup<Index + 1, Storages...>::get_unchecked_const(i));
    }
  };

  template <class ExSpace, class... Storages>
  class JoinedStorage {
  public:
    using StorageGroup = JoinedStorageGroup<0, Storages...>;

    using RefTuple = typename StorageGroup::RefTuple;

    using ConstRefTuple = typename StorageGroup::ConstRefTuple;

    JoinedStorage(Storages &... ss) : base(ss...) {}

    void each(std::function<void(const int, RefTuple)> f) {
      for (int i = 0; i < base.first_storage().size; i++) {
        std::size_t global_index = base.first_storage().global_indices[i];
        if (base.contains(global_index)) {
          auto data = base.get_unchecked(global_index);
          f(global_index, data);
        }
      }
    }

    void par_each(std::function<void(const int, RefTuple)> f) {
      auto n = base.first_storage().size;
      auto kernel = KOKKOS_LAMBDA(const int i) {
        std::size_t global_index = base.first_storage().global_indices[i];
        if (base.contains(global_index)) {
          auto data = base.get_unchecked(global_index);
          f(global_index, data);
        }
      };
      Kokkos::RangePolicy<ExSpace> linear_policy(0, n);
      Kokkos::parallel_for(linear_policy, kernel, "joined_storage_each");
    }

  private:
    JoinedStorageGroup<0, Storages...> base;
  };
} // namespace storage
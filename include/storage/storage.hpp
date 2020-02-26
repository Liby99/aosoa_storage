#pragma once

#include <optional>
#include <vector>

#include <Cabana_Core.hpp>

#include "./storage_utils.hpp"
#include "./type_transform.hpp"

namespace storage {

  template <class ExSpace, class MemSpace, std::size_t BinSize, typename... Types>
  struct Storage {

    using Self = Storage<ExSpace, MemSpace, BinSize, Types...>;

    using CabanaDataTypes = Cabana::MemberTypes<typename TypeTransform<Types>::Type...>;

    using KokkosDeviceType = Kokkos::Device<ExSpace, MemSpace>;

    using CabanaTuple = Cabana::Tuple<CabanaDataTypes>;

    using CabanaAosoa = Cabana::AoSoA<CabanaDataTypes, KokkosDeviceType, BinSize>;

    using Tuple = std::tuple<Types...>;

    using RefTuple = std::tuple<Types &...>;

    using ConstRefTuple = std::tuple<const Types &...>;

    using OptionalRefTuple = std::optional<RefTuple>;

    template <std::size_t Index>
    using TypeAt = typename ExtractTypeAt<Index, Types...>::Type;

    template <std::size_t Index>
    using OptionalRefAt = std::optional<std::reference_wrapper<TypeAt<Index>>>;

    using RefTupleExtractor = RefTupleExtractor<0, Types...>;

    using RefTupleUpdator = RefTupleUpdator<0, Types...>;

    using DataUpdator = DataUpdator<0, Types...>;

    static constexpr std::size_t N = sizeof...(Types);

    Storage() : Storage(DEFAULT_CAPACITY) {}

    Storage(std::size_t capacity) : data("storage", capacity), size(0) {
      global_indices.reserve(capacity);
    }

    std::optional<RefTuple> get(std::size_t i) {
      if (i < data_indices.size()) {
        std::optional<std::size_t> data_index = data_indices[i];
        if (data_index.has_value()) {
          return RefTupleExtractor::get(data, data_index.value());
        }
      }
      return {};
    }

    RefTuple get_unchecked(std::size_t i) {
      std::optional<std::size_t> data_index = data_indices[i];
      return RefTupleExtractor::get(data, data_index.value());
    }

    ConstRefTuple get_unchecked_const(std::size_t i) const {
      std::optional<std::size_t> data_index = data_indices[i];
      return RefTupleExtractor::get(data, data_index.value());
    }

    template <std::size_t Index>
    OptionalRefAt<Index> get_component(std::size_t i) {
      if (i < data_indices.size()) {
        std::optional<std::size_t> data_index = data_indices[i];
        if (data_index.has_value()) {
          using Transf = TypeTransform<TypeAt<Index>>;
          TypeAt<Index> &res = Transf::template get<Index>(data, data_index.value());
          return OptionalRefAt<Index>{res};
        }
      }
      return {};
    }

    template <std::size_t Index>
    TypeAt<Index> &get_component_unchecked(std::size_t i) {
      using Transf = TypeTransform<TypeAt<Index>>;
      auto data_index = data_indices[i];
      return Transf::template get<Index>(data, data_index.value());
    }

    template <std::size_t Index>
    const TypeAt<Index> &get_component_unchecked_const(std::size_t i) const {
      using Transf = TypeTransform<TypeAt<Index>>;
      auto data_index = data_indices[i];
      return Transf::template get<Index>(data, data_index.value());
    }

    bool contains(std::size_t i) const {
      if (i < data_indices.size()) {
        return data_indices[i].has_value();
      }
      return false;
    }

    void insert(std::size_t i, Types... components) {
      if (i < data_indices.size()) {
        auto data_index = data_indices[i];
        if (data_index.has_value()) {
          DataUpdator::set(data, data_index.value(), components...);
          return;
        }
      } else {
        data_indices.resize(i + CAPACITY_INCREMENT, {});
      }

      auto data_index = size++;
      data_indices[i] = data_index;

      if (data_index < global_indices.size()) {
        global_indices[data_index] = i;
      } else {
        global_indices.push_back(i);
      }

      if (data_index >= data.capacity()) {
        data.resize(data_index + CAPACITY_INCREMENT);
      }
      DataUpdator::set(data, data_index, components...);
    }

    std::size_t append(Types... components) {
      auto i = data_indices.size();
      insert(i, components...);
      return i;
    }

    template <std::size_t Index>
    void fill(TypeAt<Index> component) {
      par_each(KOKKOS_LAMBDA(int, auto data) { std::get<Index>(data) = component; });
    }

    void fill(Types... components) {
      par_each(KOKKOS_LAMBDA(int, auto data) { RefTupleUpdator::set(data, components...); });
    }

    bool update(std::size_t i, Types... components) {
      if (i < data_indices.size()) {
        auto data_index = data_indices[i];
        if (data_index.has_value()) {
          DataUpdator::set(data, data_index.value(), components...);
          return true;
        }
      }
      return false;
    }

    template <std::size_t Index>
    bool update_component(std::size_t i, TypeAt<Index> component) {
      if (i < data_indices.size()) {
        auto data_index = data_indices[i];
        if (data_index.has_value()) {
          using Transf = TypeTransform<TypeAt<Index>>;
          Transf::template set<Index>(data, data_index.value(), component);
          return true;
        }
      }
      return false;
    }

    bool remove(std::size_t i) {
      if (i < data_indices.size()) {
        auto data_index = data_indices[i];
        if (data_index.has_value()) {
          auto last_index = --size;

          // Note: Directly using Cabana's tuple type
          auto last_tuple = data.getTuple(last_index);
          data.setTuple(data_index.value(), last_tuple);

          data_indices[global_indices[last_index]] = data_index.value();
          data_indices[i] = {};

          global_indices[data_index.value()] = global_indices[last_index];

          return true;
        }
      }
      return false;
    }

    void reserve(std::size_t size) {
      global_indices.reserve(size);
      data.resize(size);
    }

    void each(std::function<void(int, RefTuple)> f) {
      for (int i = 0; i < size; i++) {
        auto global_index = global_indices[i];
        auto element = RefTupleExtractor::get(data, i);
        f(global_index, element);
      }
    }

    void par_each(std::function<void(int, RefTuple)> f) {
      auto kernel = KOKKOS_LAMBDA(const int i) {
        auto global_index = global_indices[i];
        auto element = RefTupleExtractor::get(data, i);
        f(global_index, element);
      };
      Kokkos::RangePolicy<ExSpace> linear_policy(0, size);
      Kokkos::parallel_for(linear_policy, kernel, "storage_par_each");
    }

    template <typename OtherExSpace, typename OtherMemSpace, std::size_t OtherBinSize>
    void deep_copy_to(Storage<OtherExSpace, OtherMemSpace, OtherBinSize, Types...> &other) {
      Cabana::deep_copy(data, other.data);
      other.size = size;
      other.data_indices = std::vector<std::optional<std::size_t>>(data_indices);
      other.global_indices = std::vector<std::size_t>(global_indices);
    }

  private:
    // Instance fields
    CabanaAosoa data;
    std::vector<std::size_t> global_indices;
    std::vector<std::optional<std::size_t>> data_indices;
    std::size_t size;

    // Constants for resource management
    static const std::size_t DEFAULT_CAPACITY = 100;
    static const std::size_t CAPACITY_INCREMENT = 50;
  };
} // namespace storage
#pragma once

#include <optional>
#include <vector>

#include <Cabana_Core.hpp>

#include "./storage_utils.hpp"
#include "./type_transform.hpp"

namespace storage {

  template <class ExSpace, class MemSpace, std::size_t BinSize, typename... Types>
  struct Storage {

    // Type definitions
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

    using TupleParser = TupleParser<Types...>;

    using RefTupleExtractor = RefTupleExtractor<0, Types...>;

    using RefTupleUpdator = RefTupleUpdator<0, Types...>;

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
          TypeAt<Index> &res = TypeTransform<TypeAt<Index>>::template get<Index, CabanaAosoa>(
              data, data_index.value());
          return OptionalRefAt<Index>{res};
        }
      }
      return {};
    }

    template <std::size_t Index>
    TypeAt<Index> &get_component_unchecked(std::size_t i) {
      auto data_index = data_indices[i];
      return TypeTransform<TypeAt<Index>>::template get<Index, CabanaAosoa>(data, data_index.value());
    }

    template <std::size_t Index>
    const TypeAt<Index> &get_component_unchecked_const(std::size_t i) const {
      auto data_index = data_indices[i];
      return TypeTransform<TypeAt<Index>>::template get<Index, CabanaAosoa>(data, data_index.value());
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
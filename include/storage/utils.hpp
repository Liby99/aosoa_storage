#pragma once

#ifdef STORAGE_ENABLED_CUDA
#include <nvfunctional>
#else
#include <functional>
#endif

#ifdef STORAGE_ENABLED_CUDA
#define Fn nvstd::function
#define FnFlag __host__ __device__
#else
#define Fn std::function
#define FnFlag
#endif

#include "./type_transform.hpp"

namespace storage {
  template <int Index, typename T, typename... Types>
  struct ExtractTypeAt {
    using Type = typename ExtractTypeAt<Index - 1, Types...>::Type;
  };

  template <typename T, typename... Types>
  struct ExtractTypeAt<0, T, Types...> {
    using Type = T;
  };

  template <int Index, class SliceHolder, typename... Types>
  struct UpdatorImpl {
    static KOKKOS_INLINE_FUNCTION void
    update(const SliceHolder &slice_holder, const int i, const Types &... components) {}
  };

  template <int Index, class SliceHolder, typename T, typename... Types>
  struct UpdatorImpl<Index, SliceHolder, T, Types...> {
    static KOKKOS_INLINE_FUNCTION void
    update(const SliceHolder &slice_holder, const int i, const T &c, const Types &... components) {
      TypeTransform<T>::set(slice_holder.template get<Index>(), i, c);
      UpdatorImpl<Index + 1, SliceHolder, Types...>::update(slice_holder, i, components...);
    }
  };

  template <class SliceHolder, typename... Types>
  struct Updator : public UpdatorImpl<0, SliceHolder, Types...> {};

  template <int Index, typename... Types>
  struct ToCabanaTupleImpl {
    template <typename Tuple>
    static KOKKOS_INLINE_FUNCTION void to_cabana_tuple(Tuple &target, const Types &... cs) {}
  };

  template <int Index, typename T, typename... Types>
  struct ToCabanaTupleImpl<Index, T, Types...> {
    template <typename Tuple>
    static KOKKOS_INLINE_FUNCTION void
    to_cabana_tuple(Tuple &target, const T &c, const Types &... cs) {
      TypeTransform<T>::template set<Index>(target, c);
      ToCabanaTupleImpl<Index + 1, Types...>::to_cabana_tuple(target, cs...);
    }
  };

  template <typename... Types>
  struct ToCabanaTuple {
    using MemberTypes = Cabana::MemberTypes<typename TypeTransform<Types>::To...>;

    using Tuple = Cabana::Tuple<MemberTypes>;

    static KOKKOS_INLINE_FUNCTION Tuple to_cabana(const Types &... cs) {
      Tuple target;
      ToCabanaTupleImpl<0, Types...>::to_cabana_tuple(target, cs...);
      return target;
    }
  };
} // namespace storage
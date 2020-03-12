#pragma once

#include <iostream>

#include <Cabana_Core.hpp>
#include <math/prelude.hpp>

namespace storage {
  template <typename T>
  struct TypeTransform {
    using From = T;

    using To = T;

    template <typename Slice>
    static KOKKOS_INLINE_FUNCTION From get(const Slice &slice, const std::size_t i) {
      return slice(i);
    }

    template <int Index, typename Tuple>
    static KOKKOS_INLINE_FUNCTION From get(Tuple &tuple) {
      return Cabana::get<Index>(tuple);
    }

    template <typename Slice>
    static KOKKOS_INLINE_FUNCTION void set(const Slice &slice, const std::size_t i, const From &c) {
      slice(i) = c;
    }

    template <int Index, typename Tuple>
    static KOKKOS_INLINE_FUNCTION void set(Tuple &tuple, const From &c) {
      Cabana::get<Index>(tuple) = c;
    }
  };

  template <class T>
  struct TypeTransform<math::Vector<T, 2>> {
    using From = math::Vector<T, 2>;

    using To = T[2];

    template <typename Slice>
    static KOKKOS_INLINE_FUNCTION From get(const Slice &slice, const std::size_t i) {
      return From(slice(i, 0), slice(i, 1));
    }

    template <int Index, typename Tuple>
    static KOKKOS_INLINE_FUNCTION From get(Tuple &tuple) {
      return From(Cabana::get<Index>(tuple, 0), Cabana::get<Index>(tuple, 1));
    }

    template <typename Slice>
    static KOKKOS_INLINE_FUNCTION void set(const Slice &slice, const std::size_t i, const From &c) {
      slice(i, 0) = c.x;
      slice(i, 1) = c.y;
    }

    template <int Index, typename Tuple>
    static KOKKOS_INLINE_FUNCTION void set(Tuple &tuple, const From &c) {
      Cabana::get<Index>(tuple, 0) = c.x;
      Cabana::get<Index>(tuple, 1) = c.y;
    }
  };

  template <class T>
  struct TypeTransform<math::Vector<T, 3>> {
    using From = math::Vector<T, 3>;

    using To = T[3];

    template <typename Slice>
    static KOKKOS_INLINE_FUNCTION From get(const Slice &slice, const std::size_t i) {
      return From(slice(i, 0), slice(i, 1), slice(i, 2));
    }

    template <int Index, typename Tuple>
    static KOKKOS_INLINE_FUNCTION From get(Tuple &tuple) {
      return From(Cabana::get<Index>(tuple, 0), Cabana::get<Index>(tuple, 1),
                  Cabana::get<Index>(tuple, 2));
    }

    template <typename Slice>
    static KOKKOS_INLINE_FUNCTION void set(const Slice &slice, const std::size_t i, const From &c) {
      slice(i, 0) = c.x;
      slice(i, 1) = c.y;
      slice(i, 2) = c.z;
    }

    template <int Index, typename Tuple>
    static KOKKOS_INLINE_FUNCTION void set(Tuple &tuple, const From &c) {
      Cabana::get<Index>(tuple, 0) = c.x;
      Cabana::get<Index>(tuple, 1) = c.y;
      Cabana::get<Index>(tuple, 2) = c.z;
    }
  };
} // namespace storage
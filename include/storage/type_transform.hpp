#pragma once

#include <math/prelude.hpp>
#include <Cabana_Core.hpp>

namespace storage {
  template <typename T>
  struct TypeTransform {
    using From = T;

    using To = T;

    template <typename Slice>
    static KOKKOS_INLINE_FUNCTION From get(const Slice &slice, const int i) {
      return slice(i);
    }

    template <typename Slice>
    static KOKKOS_INLINE_FUNCTION void set(const Slice &slice, const int i, const From &component) {
      slice(i) = component;
    }
  };

  template <class T>
  struct TypeTransform<storage_math::Vector<T, 2>> {
    using From = storage_math::Vector<T, 2>;

    using To = T[2];

    template <typename Slice>
    static KOKKOS_INLINE_FUNCTION From get(const Slice &slice, const int i) {
      return From(slice(i, 0), slice(i, 1));
    }

    template <typename Slice>
    static KOKKOS_INLINE_FUNCTION void set(const Slice &slice, const int i, const From &component) {
      slice(i, 0) = component(0);
      slice(i, 1) = component(1);
    }
  };

  template <class T>
  struct TypeTransform<storage_math::Vector<T, 3>> {
    using From = storage_math::Vector<T, 3>;

    using To = T[3];

    template <typename Slice>
    static KOKKOS_INLINE_FUNCTION From get(const Slice &slice, const int i) {
      return From(slice(i, 0), slice(i, 1), slice(i, 2));
    }

    template <typename Slice>
    static KOKKOS_INLINE_FUNCTION void set(const Slice &slice, const int i, const From &component) {
      slice(i, 0) = component(0);
      slice(i, 1) = component(1);
      slice(i, 2) = component(2);
    }
  };
} // namespace storage
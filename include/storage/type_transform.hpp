#pragma once

#include "math/prelude.hpp"

namespace storage {

  template <typename T>
  struct TypeTransform {
    using Type = T;

    template <std::size_t Index, typename AosoaType>
    static STORAGE_FORCE_INLINE T &get(const AosoaType &data, std::size_t i) {
      return Cabana::slice<Index, AosoaType>(data)(i);
    }

    template <std::size_t Index, typename AosoaType>
    static STORAGE_FORCE_INLINE void set(AosoaType &data, std::size_t i, T &value) {
      Cabana::slice<Index, AosoaType>(data)(i) = value;
    }
  };

  template <std::size_t Index, typename AosoaType, typename T, typename S>
  STORAGE_FORCE_INLINE S &get_from_slice(const AosoaType &data, std::size_t i) {
    T *ptr = &Cabana::slice<Index, AosoaType>(data)(i, 0);
    S *vptr = (S *)ptr;
    return *vptr;
  }

  template <std::size_t Index, typename AosoaType, typename T, typename S>
  STORAGE_FORCE_INLINE void set_to_slice(AosoaType &data, std::size_t i, const S &value) {
    T *ptr = &Cabana::slice<Index, AosoaType>(data)(i, 0);
    S *vptr = (S *)ptr;
    *vptr = value;
  }

  template <typename T, int dim>
  struct TypeTransform<math::Vector<T, dim>> {
    using Type = T[4];

    using From = math::Vector<T, dim>;

    template <std::size_t Index, typename AosoaType>
    static STORAGE_FORCE_INLINE From &get(const AosoaType &data, std::size_t i) {
      return get_from_slice<Index, AosoaType, T, From>(data, i);
    }

    template <std::size_t Index, typename AosoaType>
    static STORAGE_FORCE_INLINE void set(AosoaType &data, std::size_t i, const From &value) {
      set_to_slice<Index, AosoaType, T, From>(data, i, value);
    }
  };

  template <typename T, int dim>
  struct TypeTransform<math::Matrix<T, dim>> {
    using Type = T[dim * 4];

    using From = math::Matrix<T, dim>;

    template <std::size_t Index, typename AosoaType>
    static STORAGE_FORCE_INLINE From &get(const AosoaType &data, std::size_t i) {
      return get_from_slice<Index, AosoaType, T, From>(data, i);
    }

    template <std::size_t Index, typename AosoaType>
    static STORAGE_FORCE_INLINE void set(AosoaType &data, std::size_t i, const From &value) {
      set_to_slice<Index, AosoaType, T, From>(data, i, value);
    }
  };
} // namespace storage
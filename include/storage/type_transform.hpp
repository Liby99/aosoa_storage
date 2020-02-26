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

  template <typename T, int dim>
  struct TypeTransform<math::Vector<T, dim>> {
    using Type = T[4];

    using From = math::Vector<T, dim>;

    template <std::size_t Index, typename AosoaType>
    static STORAGE_FORCE_INLINE math::Vector<T, dim> &get(const AosoaType &data, std::size_t i) {
      T *ptr = &Cabana::slice<Index, AosoaType>(data)(i, 0);
      math::Vector<T, dim> *vptr = (math::Vector<T, dim> *)ptr;
      return *vptr;
    }

    template <std::size_t Index, typename AosoaType>
    static STORAGE_FORCE_INLINE void set(AosoaType &data, std::size_t i, From &value) {
      auto slice = Cabana::slice<Index, AosoaType>(data);
      slice(i, 0) = value.x;
      slice(i, 1) = value.y;
      slice(i, 2) = value.z;
      slice(i, 3) = value.w;
    }
  };

  template <typename T>
  struct TypeTransform<math::Matrix<T, 2>> {
    using Type = T[8];

    using From = math::Matrix<T, 2>;

    template <std::size_t Index, typename AosoaType>
    static STORAGE_FORCE_INLINE From &get(const AosoaType &data, std::size_t i) {
      T *ptr = &Cabana::slice<Index, AosoaType>(data)(i, 0);
      From *vptr = (From *)ptr;
      return *vptr;
    }

    template <std::size_t Index, typename AosoaType>
    static STORAGE_FORCE_INLINE void set(AosoaType &data, std::size_t i, From &value) {
      auto slice = Cabana::slice<Index, AosoaType>(data);
      slice(i, 0) = value.data[0](0);
      slice(i, 1) = value.data[0](1);
      slice(i, 2) = value.data[0](2);
      slice(i, 3) = value.data[0](3);
      slice(i, 4) = value.data[1](0);
      slice(i, 5) = value.data[1](1);
      slice(i, 6) = value.data[1](2);
      slice(i, 7) = value.data[1](3);
    }
  };

  template <typename T>
  struct TypeTransform<math::Matrix<T, 3>> {
    using Type = T[12];

    using From = math::Matrix<T, 3>;

    template <std::size_t Index, typename AosoaType>
    static STORAGE_FORCE_INLINE From &get(const AosoaType &data, std::size_t i) {
      T *ptr = &Cabana::slice<Index, AosoaType>(data)(i, 0);
      From *vptr = (From *)ptr;
      return *vptr;
    }

    template <std::size_t Index, typename AosoaType>
    static STORAGE_FORCE_INLINE void set(AosoaType &data, std::size_t i, From &value) {
      auto slice = Cabana::slice<Index, AosoaType>(data);
      slice(i, 0) = value.data[0](0);
      slice(i, 1) = value.data[0](1);
      slice(i, 2) = value.data[0](2);
      slice(i, 3) = value.data[0](3);
      slice(i, 4) = value.data[1](0);
      slice(i, 5) = value.data[1](1);
      slice(i, 6) = value.data[1](2);
      slice(i, 7) = value.data[1](3);
      slice(i, 8) = value.data[2](0);
      slice(i, 9) = value.data[2](1);
      slice(i, 10) = value.data[2](2);
      slice(i, 11) = value.data[2](3);
    }
  };

  template <typename T>
  struct TypeTransform<math::Matrix<T, 4>> {
    using Type = T[16];

    using From = math::Matrix<T, 4>;

    template <std::size_t Index, typename AosoaType>
    static STORAGE_FORCE_INLINE From &get(const AosoaType &data, std::size_t i) {
      T *ptr = &Cabana::slice<Index, AosoaType>(data)(i, 0);
      From *vptr = (From *)ptr;
      return *vptr;
    }

    template <std::size_t Index, typename AosoaType>
    static STORAGE_FORCE_INLINE void set(AosoaType &data, std::size_t i, From &value) {
      auto slice = Cabana::slice<Index, AosoaType>(data);
      slice(i, 0) = value.data[0](0);
      slice(i, 1) = value.data[0](1);
      slice(i, 2) = value.data[0](2);
      slice(i, 3) = value.data[0](3);
      slice(i, 4) = value.data[1](0);
      slice(i, 5) = value.data[1](1);
      slice(i, 6) = value.data[1](2);
      slice(i, 7) = value.data[1](3);
      slice(i, 8) = value.data[2](0);
      slice(i, 9) = value.data[2](1);
      slice(i, 10) = value.data[2](2);
      slice(i, 11) = value.data[2](3);
      slice(i, 12) = value.data[3](0);
      slice(i, 13) = value.data[3](1);
      slice(i, 14) = value.data[3](2);
      slice(i, 15) = value.data[3](3);
    }
  };

} // namespace storage
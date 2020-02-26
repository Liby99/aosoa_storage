#pragma once

#include "math/prelude.hpp"

namespace storage {

  template <typename T>
  struct TypeTransform {
    using Type = T;

    template <std::size_t Index, typename AosoaType>
    static T &get(AosoaType &data, std::size_t i) {
      return Cabana::slice<Index, AosoaType>(data)(i);
    }
  };

  template <typename T, int dim>
  struct TypeTransform<math::Vector<T, dim>> {
    using Type = T[4];

    using From = math::Vector<T, dim>;

    template <std::size_t Index, typename AosoaType>
    static math::Vector<T, dim> &get(AosoaType &data, std::size_t i) {
      T *ptr = &Cabana::slice<Index, AosoaType>(data)(i, 0);
      math::Vector<T, dim> *vptr = (math::Vector<T, dim> *) ptr;
      return *vptr;
    }
  };

  template <typename T>
  struct TypeTransform<math::Matrix<T, 2>> {
    using Type = T[8];

    using From = math::Matrix<T, 2>;

    template <std::size_t Index, typename AosoaType>
    static From &get(AosoaType &data, std::size_t i) {
      T *ptr = &Cabana::slice<Index, AosoaType>(data)(i, 0);
      From *vptr = (From *) ptr;
      return *vptr;
    }
  };

  template <typename T>
  struct TypeTransform<math::Matrix<T, 3>> {
    using Type = T[12];

    using From = math::Matrix<T, 3>;

    template <std::size_t Index, typename AosoaType>
    static From &get(AosoaType &data, std::size_t i) {
      T *ptr = &Cabana::slice<Index, AosoaType>(data)(i, 0);
      From *vptr = (From *) ptr;
      return *vptr;
    }
  };

  template <typename T>
  struct TypeTransform<math::Matrix<T, 4>> {
    using Type = T[16];

    using From = math::Matrix<T, 4>;

    template <std::size_t Index, typename AosoaType>
    static From &get(AosoaType &data, std::size_t i) {
      T *ptr = &Cabana::slice<Index, AosoaType>(data)(i, 0);
      From *vptr = (From *) ptr;
      return *vptr;
    }
  };

} // namespace storage
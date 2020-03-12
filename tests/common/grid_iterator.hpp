#pragma once

#include <math/prelude.hpp>
#include "./iterator_range.hpp"

template <int D>
struct GridIterator {};

template <>
struct GridIterator<2> {
  int x, y, i, j;

  GridIterator(int x, int y) : x(x), y(y), i(0), j(0) {}

  GridIterator(int x, int y, bool _) : x(x), y(y), i(0), j(y) {}

  std::size_t size() const {
    return x * y;
  }

  static IteratorRange<GridIterator<2>> range(int x, int y) {
    return make_range(GridIterator<2>(x, y), GridIterator<2>(x, y, true));
  }

  static IteratorRange<GridIterator<2>> range(math::Vector<int, 2> size) {
    return make_range(GridIterator<2>(size.x, size.y),
                      GridIterator<2>(size.x, size.y, true));
  }

  math::Vector<int, 2> operator*() {
    return math::Vector<int, 2>(i, j);
  }

  void operator++() {
    if (i < x - 1) {
      i++;
    } else {
      i = 0;
      j++;
    }
  }

  bool operator!=(GridIterator<2> &other) {
    return i != other.i || j != other.j;
  }
};

template <>
struct GridIterator<3> {
  int x, y, z, i, j, k;

  GridIterator(int x, int y, int z) : x(x), y(y), z(z), i(0), j(0), k(0) {}

  GridIterator(int x, int y, int z, bool _)
      : x(x), y(y), z(z), i(0), j(0), k(z) {}

  std::size_t size() const {
    return x * y * z;
  }

  static IteratorRange<GridIterator<3>> range(int x, int y, int z) {
    return make_range(GridIterator<3>(x, y, z), GridIterator<3>(x, y, z, true));
  }

  static IteratorRange<GridIterator<3>> range(math::Vector<int, 3> size) {
    return make_range(GridIterator<3>(size.x, size.y, size.z),
                      GridIterator<3>(size.x, size.y, size.z, true));
  }

  math::Vector<int, 3> operator*() {
    return math::Vector<int, 3>(i, j, k);
  }

  void operator++() {
    if (i < x - 1) {
      i++;
    } else {
      i = 0;
      if (j < y - 1) {
        j++;
      } else {
        j = 0;
        k++;
      }
    }
  }

  bool operator!=(GridIterator<3> &other) {
    return i != other.i || j != other.j || k != other.k;
  }
};
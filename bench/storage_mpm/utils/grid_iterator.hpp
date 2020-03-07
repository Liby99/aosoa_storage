#pragma once

#include "./iterator_range.hpp"

template <int D>
struct GridIterator {};

template <>
struct GridIterator<2> {
  int x, y, i, j;

  GridIterator(int x, int y) : x(x), y(y), i(0), j(0) {}

  GridIterator(int x, int y, bool _) : x(x), y(y), i(0), j(y) {}

  static IteratorRange<GridIterator<2>> range(int x, int y) {
    return make_range(GridIterator<2>(x, y), GridIterator<2>(x, y, true));
  }

  std::tuple<int, int> operator*() {
    return std::make_tuple(i, j);
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

  GridIterator(int x, int y, int z, bool _) : x(x), y(y), z(z), i(0), j(0), k(z) {}

  std::tuple<int, int, int> operator*() {
    return std::make_tuple(i, j, k);
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
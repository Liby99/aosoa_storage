#pragma once

#include "../config.hpp"
#include "./math/vector.hpp"

template <typename T>
struct TypeTransform {
  using From = T;

  using To = T;

  template <typename Slice>
  static From fetch(const Slice &slice, int i) {
    return slice(i);
  }

  template <typename Slice>
  static From fetch(const Slice &slice, int s, int a) {
    return slice.access(s, a);
  }

  template <typename Slice>
  static void store(const Slice &slice, int i, const From &data) {
    slice(i) = data;
  }

  template <typename Slice>
  static void store(const Slice &slice, int s, int a, const From &data) {
    slice.access(s, a) = data;
  }
};

template <class T>
struct TypeTransform<Vector<T, 2>> {
  using From = Vector<T, 2>;

  using To = T[2];

  template <typename Slice>
  static From fetch(const Slice &slice, int i) {
    return From(slice(i, 0), slice(i, 1));
  }

  template <typename Slice>
  static From fetch(const Slice &slice, int s, int a) {
    return From(slice.access(s, a, 0), slice.access(s, a, 1));
  }

  template <typename Slice>
  static void store(const Slice &slice, int i, const From &data) {
    slice(i, 0) = data.x;
    slice(i, 1) = data.y;
  }

  template <typename Slice>
  static void store(const Slice &slice, int s, int a, const From &data) {
    slice.access(s, a, 0) = data.x;
    slice.access(s, a, 1) = data.y;
  }
};

template <class T>
struct TypeTransform<Vector<T, 3>> {
  using From = Vector<T, 3>;

  using To = T[3];

  template <typename Slice>
  static From fetch(const Slice &slice, int i) {
    return From(slice(i, 0), slice(i, 1), slice(i, 2));
  }

  template <typename Slice>
  static From fetch(const Slice &slice, int s, int a) {
    return From(slice.access(s, a, 0), slice.access(s, a, 1), slice.access(s, a, 2));
  }

  template <typename Slice>
  static void store(const Slice &slice, int i, const From &data) {
    slice(i, 0) = data.x;
    slice(i, 1) = data.y;
    slice(i, 2) = data.z;
  }

  template <typename Slice>
  static void store(const Slice &slice, int s, int a, const From &data) {
    slice.access(s, a, 0) = data.x;
    slice.access(s, a, 1) = data.y;
    slice.access(s, a, 2) = data.z;
  }
};

template <class T>
struct TypeTransform<Vector<T, 4>> {
  using From = Vector<T, 4>;

  using To = T[4];

  template <typename Slice>
  static From fetch(const Slice &slice, int i) {
    return From(slice(i, 0), slice(i, 1), slice(i, 2), slice(i, 3));
  }

  template <typename Slice>
  static From fetch(const Slice &slice, int s, int a) {
    return From(slice.access(s, a, 0), slice.access(s, a, 1), slice.access(s, a, 2), slice.access(s, a, 3));
  }

  template <typename Slice>
  static void store(const Slice &slice, int i, const From &data) {
    slice(i, 0) = data.x;
    slice(i, 1) = data.y;
    slice(i, 2) = data.z;
    slice(i, 3) = data.w;
  }

  template <typename Slice>
  static void store(const Slice &slice, int s, int a, const From &data) {
    slice.access(s, a, 0) = data.x;
    slice.access(s, a, 1) = data.y;
    slice.access(s, a, 2) = data.z;
    slice.access(s, a, 3) = data.w;
  }
};

template <int Index, typename T, typename... Types>
struct ExtractTypeAt {
  using Type = typename ExtractTypeAt<Index - 1, Types...>::Type;
};

template <typename T, typename... Types>
struct ExtractTypeAt<0, T, Types...> {
  using Type = T;
};

struct Range {
  int start, length;
};

template <typename... Types>
struct ElementHandle {
  using MemberTypes = Cabana::MemberTypes<typename TypeTransform<Types>::To ...>;

  using CabanaAoSoA = Cabana::AoSoA<MemberTypes, KokkosDevice, BIN_SIZE>;

  template <int Index>
  using TypeAt = typename ExtractTypeAt<Index, Types...>::Type;

  CabanaAoSoA &data;

  int index;

  ElementHandle(CabanaAoSoA &data, int index) : data(data), index(index) {}

  template <int Index>
  TypeAt<Index> fetch() const {
    auto slice = Cabana::slice<Index>(data);
    TypeTransform<TypeAt<Index>>::fetch(slice, index);
  }

  template <int Index>
  void store(const TypeAt<Index> &comp) {
    auto slice = Cabana::slice<Index>(data);
    TypeTransform<TypeAt<Index>>::store(slice, index, comp);
  }
};

template <typename... Types>
struct Storage {
  using MemberTypes = Cabana::MemberTypes<typename TypeTransform<Types>::To ...>;

  using CabanaAoSoA = Cabana::AoSoA<MemberTypes, KokkosDevice, BIN_SIZE>;

  CabanaAoSoA data;

  int size;

  Storage(int capacity) : data("storage", capacity) {}

  template <typename Iter, typename F>
  Range insert_iter(Iter iter, F callback) {
    int start = size;
    ElementHandle<Types...> handle(data, size);
    for (auto data : iter) {
      handle.index = size;
      callback(data, handle);
      size++;
    }
    int length = size - start;
    return Range { start, length };
  }
};
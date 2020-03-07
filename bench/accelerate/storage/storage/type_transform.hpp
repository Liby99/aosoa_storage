#pragma once

#include "../math/vector.hpp"

template <typename T>
struct TypeTransform {
  using From = T;

  using To = T;

  template <typename Slice>
  static inline From fetch(const Slice &slice, int i) {
    return slice(i);
  }

  template <typename Slice>
  static inline From fetch(const Slice &slice, int s, int a) {
    return slice.access(s, a);
  }

  template <typename Slice>
  static inline void store(const Slice &slice, int i, const From &data) {
    slice(i) = data;
  }

  template <typename Slice>
  static inline void store(const Slice &slice, int s, int a, const From &data) {
    slice.access(s, a) = data;
  }
};

template <class T>
struct TypeTransform<Vector<T, 2>> {
  using From = Vector<T, 2>;

  using To = T[2];

  template <typename Slice>
  static inline From fetch(const Slice &slice, int i) {
    return From(slice(i, 0), slice(i, 1));
  }

  template <typename Slice>
  static inline From fetch(const Slice &slice, int s, int a) {
    return From(slice.access(s, a, 0), slice.access(s, a, 1));
  }

  template <typename Slice>
  static inline void store(const Slice &slice, int i, const From &data) {
    slice(i, 0) = data.x;
    slice(i, 1) = data.y;
  }

  template <typename Slice>
  static inline void store(const Slice &slice, int s, int a, const From &data) {
    slice.access(s, a, 0) = data.x;
    slice.access(s, a, 1) = data.y;
  }
};

template <class T>
struct TypeTransform<Vector<T, 3>> {
  using From = Vector<T, 3>;

  using To = T[3];

  template <typename Slice>
  static inline From fetch(const Slice &slice, int i) {
    return From(slice(i, 0), slice(i, 1), slice(i, 2));
  }

  template <typename Slice>
  static inline From fetch(const Slice &slice, int s, int a) {
    return From(slice.access(s, a, 0), slice.access(s, a, 1),
                slice.access(s, a, 2));
  }

  template <typename Slice>
  static inline void store(const Slice &slice, int i, const From &data) {
    slice(i, 0) = data.x;
    slice(i, 1) = data.y;
    slice(i, 2) = data.z;
  }

  template <typename Slice>
  static inline void store(const Slice &slice, int s, int a, const From &data) {
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
  static inline From fetch(const Slice &slice, int i) {
    return From(slice(i, 0), slice(i, 1), slice(i, 2), slice(i, 3));
  }

  template <typename Slice>
  static inline From fetch(const Slice &slice, int s, int a) {
    return From(slice.access(s, a, 0), slice.access(s, a, 1),
                slice.access(s, a, 2), slice.access(s, a, 3));
  }

  template <typename Slice>
  static inline void store(const Slice &slice, int i, const From &data) {
    slice(i, 0) = data.x;
    slice(i, 1) = data.y;
    slice(i, 2) = data.z;
    slice(i, 3) = data.w;
  }

  template <typename Slice>
  static inline void store(const Slice &slice, int s, int a, const From &data) {
    slice.access(s, a, 0) = data.x;
    slice.access(s, a, 1) = data.y;
    slice.access(s, a, 2) = data.z;
    slice.access(s, a, 3) = data.w;
  }
};
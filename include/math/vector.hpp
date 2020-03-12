#pragma once

#include <Kokkos_Core.hpp>

namespace math {

  template <class T, int D>
  struct Vector {};

  template <class T>
  struct Vector<T, 2> {
    union {
      struct {
        T x, y;
      };
      T data[2];
    };

    KOKKOS_FUNCTION Vector() : Vector(0) {}

    KOKKOS_FUNCTION Vector(T x) : Vector(x, x) {}

    KOKKOS_FUNCTION Vector(T x, T y) : x(x), y(y) {}

    KOKKOS_INLINE_FUNCTION T operator()(int i) const {
      return data[i];
    }

    KOKKOS_INLINE_FUNCTION Vector<T, 2> operator+(Vector<T, 2> other) const {
      return Vector<T, 2>(x + other.x, y + other.y);
    }

    KOKKOS_INLINE_FUNCTION Vector<T, 2> operator-(Vector<T, 2> other) const {
      return Vector<T, 2>(x - other.x, y - other.y);
    }

    KOKKOS_INLINE_FUNCTION Vector<T, 2> operator*(T other) const {
      return Vector<T, 2>(x * other, y * other);
    }

    KOKKOS_INLINE_FUNCTION Vector<T, 2> operator/(T other) const {
      return Vector<T, 2>(x / other, y / other);
    }

    KOKKOS_INLINE_FUNCTION T dot(Vector<T, 2> other) const {
      return x * other.x + y * other.y;
    }

    KOKKOS_INLINE_FUNCTION void operator+=(Vector<T, 2> other) {
      x += other.x;
      y += other.y;
    }

    KOKKOS_INLINE_FUNCTION void operator-=(Vector<T, 2> other) {
      x -= other.x;
      y -= other.y;
    }

    KOKKOS_INLINE_FUNCTION void operator*=(T other) {
      x *= other;
      y *= other;
    }

    KOKKOS_INLINE_FUNCTION void operator/=(T other) {
      x /= other;
      y /= other;
    }

    template <typename U>
    KOKKOS_INLINE_FUNCTION Vector<U, 2> cast() const {
      return Vector<U, 2>((U)x, (U)y);
    }
  };

  template <class T>
  struct Vector<T, 3> {
    union {
      struct {
        T x, y, z;
      };
      T data[3];
    };

    KOKKOS_FUNCTION Vector() : Vector(0) {}

    KOKKOS_FUNCTION Vector(T x) : Vector(x, x, x) {}

    KOKKOS_FUNCTION Vector(T x, T y, T z) : x(x), y(y), z(z) {}

    KOKKOS_INLINE_FUNCTION T operator()(int i) const {
      return data[i];
    }

    KOKKOS_INLINE_FUNCTION Vector<T, 3> operator+(const Vector<T, 3> &other) const {
      return Vector<T, 3>(x + other.x, y + other.y, z + other.z);
    }

    KOKKOS_INLINE_FUNCTION Vector<T, 3> operator-(const Vector<T, 3> &other) const {
      return Vector<T, 3>(x - other.x, y - other.y, z - other.z);
    }

    KOKKOS_INLINE_FUNCTION Vector<T, 3> operator*(const T other) const {
      return Vector<T, 3>(x * other, y * other, z * other);
    }

    KOKKOS_INLINE_FUNCTION Vector<T, 3> operator/(const T other) const {
      return Vector<T, 3>(x / other, y / other, z / other);
    }

    KOKKOS_INLINE_FUNCTION T dot(const Vector<T, 3> &other) const {
      return x * other.x + y * other.y + z * other.z;
    }

    KOKKOS_INLINE_FUNCTION void operator+=(const Vector<T, 3> &other) {
      x += other.x;
      y += other.y;
      z += other.z;
    }

    KOKKOS_INLINE_FUNCTION void operator-=(const Vector<T, 3> &other) {
      x -= other.x;
      y -= other.y;
      z -= other.z;
    }

    KOKKOS_INLINE_FUNCTION void operator*=(T other) {
      x *= other;
      y *= other;
      z *= other;
    }

    KOKKOS_INLINE_FUNCTION void operator/=(T other) {
      x /= other;
      y /= other;
      z /= other;
    }

    template <typename U>
    KOKKOS_INLINE_FUNCTION Vector<U, 3> cast() const {
      return Vector<U, 3>((U)x, (U)y, (U)z);
    }
  };

} // namespace math
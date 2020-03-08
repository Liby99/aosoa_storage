#pragma once

namespace storage_math {

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

    inline Vector() : Vector(0) {}

    inline Vector(T x) : Vector(x, x) {}

    inline Vector(T x, T y) : x(x), y(y) {}

    inline T operator()(int i) const {
      return data[i];
    }

    inline Vector<T, 2> operator+(Vector<T, 2> other) const {
      return Vector<T, 2>(x + other.x, y + other.y);
    }

    inline Vector<T, 2> operator-(Vector<T, 2> other) const {
      return Vector<T, 2>(x - other.x, y - other.y);
    }

    inline Vector<T, 2> operator*(T other) const {
      return Vector<T, 2>(x * other, y * other);
    }

    inline Vector<T, 2> operator/(T other) const {
      return Vector<T, 2>(x / other, y / other);
    }

    inline T dot(Vector<T, 2> other) const {
      return x * other.x + y * other.y;
    }

    inline void operator+=(Vector<T, 2> other) {
      x += other.x;
      y += other.y;
    }

    inline void operator-=(Vector<T, 2> other) {
      x -= other.x;
      y -= other.y;
    }

    inline void operator*=(T other) {
      x *= other;
      y *= other;
    }

    inline void operator/=(T other) {
      x /= other;
      y /= other;
    }

    template <typename U>
    inline Vector<U, 2> cast() const {
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

    inline Vector() : Vector(0) {}

    inline Vector(T x) : Vector(x, x, x) {}

    inline Vector(T x, T y, T z) : x(x), y(y), z(z) {}

    inline T operator()(int i) const {
      return data[i];
    }

    inline Vector<T, 3> operator+(Vector<T, 3> other) const {
      return Vector<T, 3>(x + other.x, y + other.y, z + other.z);
    }

    inline Vector<T, 3> operator-(Vector<T, 3> other) const {
      return Vector<T, 3>(x - other.x, y - other.y, z - other.z);
    }

    inline Vector<T, 3> operator*(T other) const {
      return Vector<T, 3>(x * other, y * other, z * other);
    }

    inline Vector<T, 3> operator/(T other) const {
      return Vector<T, 3>(x / other, y / other, z / other);
    }

    inline T dot(Vector<T, 3> other) const {
      return x * other.x + y * other.y + z * other.z;
    }

    inline void operator+=(Vector<T, 3> other) {
      x += other.x;
      y += other.y;
      z += other.z;
    }

    inline void operator-=(Vector<T, 3> other) {
      x -= other.x;
      y -= other.y;
      z -= other.z;
    }

    inline void operator*=(T other) {
      x *= other;
      y *= other;
      z *= other;
    }

    inline void operator/=(T other) {
      x /= other;
      y /= other;
      z /= other;
    }

    template <typename U>
    inline Vector<U, 2> cast() const {
      return Vector<U, 2>((U)x, (U)y);
    }
  };

} // namespace math
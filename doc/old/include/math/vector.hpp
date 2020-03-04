#pragma once

namespace math {

  template <typename T, int dim>
  struct Vector {
    union {
      T data[4];
      struct {
        T x, y, z, w;
      };
    };

    explicit Vector() : x(0), y(0), z(0), w(0) {}

    explicit Vector(T x) : x(x), y(x), z(x), w(x) {}

    explicit Vector(T x, T y) : x(x), y(y), z(0), w(0) {}

    explicit Vector(T x, T y, T z) : x(x), y(y), z(z), w(0) {}

    explicit Vector(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

    T &operator()(int d) {
      return data[d];
    }

    const T &operator()(int d) const {
      return data[d];
    }

    Vector<T, dim> operator+(const Vector<T, dim> &rhs) const {
      return Vector<T, dim>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    Vector<T, dim> operator-(const Vector<T, dim> &rhs) const {
      return Vector<T, dim>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    Vector<T, dim> operator*(T s) const {
      return Vector<T, dim>(x * s, y * s, z * s, w * s);
    }
  };
} // namespace math
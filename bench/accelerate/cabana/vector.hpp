#pragma once

template <class T, int D>
struct Vector {};

template <class T>
struct Vector<T, 2> {
  T x, y;

  Vector() : Vector(0) {}

  Vector(T x) : Vector(x, x) {}

  Vector(T x, T y) : x(x), y(y) {}

  Vector<T, 2> operator+(Vector<T, 2> other) const {
    return Vector<T, 2>(x + other.x, y + other.y);
  }

  Vector<T, 2> operator-(Vector<T, 2> other) const {
    return Vector<T, 2>(x - other.x, y - other.y);
  }

  void operator+=(Vector<T, 2> other) {
    x += other.x;
    y += other.y;
  }

  void operator-=(Vector<T, 2> other) {
    x -= other.x;
    y -= other.y;
  }

  Vector<T, 2> operator*(T other) const {
    return Vector<T, 2>(x * other, y * other);
  }

  template <typename U>
  Vector<U, 2> cast() const {
    return Vector<U, 2>((U)x, (U)y);
  }
};

template <class T>
struct Vector<T, 3> {
  T x, y, z;

  Vector() : Vector(0) {}

  Vector(T x) : Vector(x, x, x) {}

  Vector(T x, T y, T z) : x(x), y(y), z(z) {}

  Vector<T, 3> operator+(Vector<T, 3> other) const {
    return Vector<T, 3>(x + other.x, y + other.y, z + other.z);
  }

  Vector<T, 3> operator-(Vector<T, 3> other) const {
    return Vector<T, 3>(x - other.x, y - other.y, z - other.z);
  }

  void operator+=(Vector<T, 3> other) {
    x += other.x;
    y += other.y;
    z += other.z;
  }

  void operator-=(Vector<T, 3> other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
  }

  Vector<T, 3> operator*(T other) const {
    return Vector<T, 3>(x * other, y * other, z * other);
  }

  template <typename U>
  Vector<U, 3> cast() const {
    return Vector<U, 3>((U)x, (U)y, (U)z);
  }
};

template <class T>
struct Vector<T, 4> {
  T x, y, z, w;

  Vector() : Vector(0) {}

  Vector(T x) : Vector(x, x, x, x) {}

  Vector(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

  Vector<T, 4> operator+(Vector<T, 4> other) const {
    return Vector<T, 4>(x + other.x, y + other.y, z + other.z, w + other.w);
  }

  Vector<T, 4> operator-(Vector<T, 4> other) const {
    return Vector<T, 4>(x - other.x, y - other.y, z - other.z, w - other.w);
  }

  void operator+=(Vector<T, 2> other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
  }

  void operator-=(Vector<T, 2> other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
  }

  Vector<T, 4> operator*(T other) const {
    return Vector<T, 4>(x * other, y * other, z * other, w * other);
  }

  template <typename U>
  Vector<U, 4> cast() const {
    return Vector<U, 4>((U)x, (U)y, (U)z, (U)w);
  }
};
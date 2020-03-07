#pragma once

template <class T, int D>
struct Vector {};

template <class T>
struct Vector<T, 2> {
  T x, y;

  Vector() : Vector(0) {}

  Vector(T x) : Vector(x, x) {}

  Vector(T x, T y) : x(x), y(y) {}

  Vector<T, 2> operator+(Vector<T, 2> other) {
    return Vector<T, 2>(x + other.x, y + other.y);
  }

  Vector<T, 2> operator-(Vector<T, 2> other) {
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

  Vector<T, 2> operator*(T other) {
    return Vector<T, 2>(x * other, y * other);
  }
};

template <class T>
struct Vector<T, 3> {
  T x, y, z;

  Vector() : Vector(0) {}

  Vector(T x) : Vector(x, x, x) {}

  Vector(T x, T y, T z) : x(x), y(y), z(z) {}
};

template <class T>
struct Vector<T, 4> {
  T x, y, z, w;

  Vector() : Vector(0) {}

  Vector(T x) : Vector(x, x, x, x) {}

  Vector(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
};
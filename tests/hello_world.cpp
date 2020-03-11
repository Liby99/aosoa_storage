#include <stdio.h>

template <class T>
void f() {
  printf("Raw f\n");
}

template <>
void f<int>() {
  printf("int f\n");
}

template <>
void f<float>() {
  printf("float f\n");
}

template <class T>
void g() {
  f<T>();
}

int main() {
  printf("Hello world!\n");
  g<float>();
  g<int>();
}
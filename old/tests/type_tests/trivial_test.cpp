#include "math/prelude.hpp"
#include <assert.h>
#include <stdio.h>
#include <type_traits>

using namespace math;

struct MyVector2f {
  float x, y;
};

int main() {
  printf("Vector2f: %d\n", std::is_trivial<Vector2f>::value); // 0 (false)
  printf("MyVector2f: %d\n", std::is_trivial<MyVector2f>::value); // 1 (true)
}
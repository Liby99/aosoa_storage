#include <assert.h>
#include "math/prelude.hpp"

using namespace math;

int main() {
  assert(sizeof(Vector<float, 2>) == 16);
  assert(sizeof(Vector<float, 3>) == 16);
  assert(sizeof(Vector<float, 4>) == 16);
  assert(sizeof(Matrix<float, 2>) == 32);
  assert(sizeof(Matrix<float, 3>) == 48);
  assert(sizeof(Matrix<float, 4>) == 64);

  assert(sizeof(Vector<double, 2>) == 32);
  assert(sizeof(Vector<double, 3>) == 32);
  assert(sizeof(Vector<double, 4>) == 32);
  assert(sizeof(Matrix<double, 2>) == 64);
  assert(sizeof(Matrix<double, 3>) == 96);
  assert(sizeof(Matrix<double, 4>) == 128);
}
#include <assert.h>
#include <stdio.h>

#include "math/prelude.hpp"
#include "storage/prelude.hpp"

using namespace storage;

using namespace math;

template <typename... Types>
using BaseStorage = Storage<Kokkos::Serial, Kokkos::HostSpace, 4, Types...>;

void test_1() {
  BaseStorage<Vector3f, float> store_1;
  BaseStorage<float, float, Matrix3f> store_2;
  store_1.join(store_2);
}

int main() {
  Kokkos::initialize();
  test_1();
  Kokkos::finalize();
}
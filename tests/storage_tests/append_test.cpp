#include <assert.h>
#include <stdio.h>

#include "storage/prelude.hpp"
#include "math/prelude.hpp"

using namespace storage;

using namespace math;

template <typename... Types>
using BaseStorage = Storage<Kokkos::Serial, Kokkos::HostSpace, 4, Types...>;

void test() {
  using Storage = BaseStorage<Vector3f, Vector3f, float>;
  Storage store;

  for (int i = 0; i < 100; i++) {
    store.append(Vector3f(10.0), Vector3f(13.0), 5);
  }

  store.each([](int, auto data) {
    auto &[x, v, m] = data;
    assert(x.x == 10.0 && x.y == 10.0 && x.z == 10.0 && x.w == 10.0);
    assert(v.x == 13.0 && v.y == 13.0 && v.z == 13.0 && v.w == 13.0);
    assert(m == 5.0);
  });
}

int main() {
  Kokkos::initialize();
  test();
  Kokkos::finalize();
}
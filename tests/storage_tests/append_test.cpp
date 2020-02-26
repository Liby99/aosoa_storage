#include <assert.h>
#include <stdio.h>

#include "math/prelude.hpp"
#include "storage/prelude.hpp"

using namespace storage;

using namespace math;

template <typename... Types>
using BaseStorage = Storage<Kokkos::Serial, Kokkos::HostSpace, 4, Types...>;

void test_1() {
  using Storage = BaseStorage<Vector3f, Vector3f, float>;

  Storage store;

  for (int i = 0; i < 100; i++) {
    store.append(Vector3f(10.0, 15.0, 20.0, 40.0), Vector3f(13.0, 14.0, 15.0, 16.0), 5);
  }

  store.each([](int, auto data) {
    auto &[x, v, m] = data;
    assert(x.x == 10.0 && x.y == 15.0 && x.z == 20.0 && x.w == 40.0);
    assert(v.x == 13.0 && v.y == 14.0 && v.z == 15.0 && v.w == 16.0);
    assert(m == 5.0);
  });

  store.each([](int i, auto data) {
    auto &[x, v, m] = data;
    x.z = i;
    m = 100.0;
  });

  store.each([](int i, auto data) {
    auto &[x, v, m] = data;
    assert(x.x == 10.0 && x.y == 15.0 && x.z == i && x.w == 40.0);
    assert(v.x == 13.0 && v.y == 14.0 && v.z == 15.0 && v.w == 16.0);
    assert(m == 100.0);
  });
}

void test_2() {
  using Storage = BaseStorage<Matrix3f, float>;

  Storage store;

  for (int i = 0; i < 100; i++) {
    store.append(Matrix3f(1.0), i);
  }

  store.each([](int i, auto data) {
    auto &[m, f] = data;
    assert(m.data[0](0) == 1.0);
    assert(m.data[0](1) == 0.0);
    assert(m.data[0](2) == 0.0);
    assert(m.data[0](3) == 0.0);
    assert(f == i);
  });

  store.each([](int i, auto data) {
    auto &[m, f] = data;
    for (int j = 0; j < 3; j++) {
      m.data[j](j) = i;
    }
    f = 0.0;
  });

  store.each([](int i, auto data) {
    auto &[m, f] = data;
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 3; k++) {
        if (j == k) {
          assert(m.data[j](k) == i);
        } else {
          assert(m.data[j](k) == 0);
        }
      }
    }
    assert(f == 0.0);
  });
}

int main() {
  Kokkos::initialize();
  test_1();
  test_2();
  Kokkos::finalize();
}
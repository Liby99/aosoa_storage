#include "math/prelude.hpp"
#include "storage/prelude.hpp"

using namespace storage;

using namespace math;

void type_1() {
  Storage<Kokkos::Serial, Kokkos::HostSpace, 4, int, int, float> store;
}

void type_2() {
  Storage<Kokkos::Serial, Kokkos::HostSpace, 4, Vector<float, 3>> store;
  auto elem = store.get(10);
  auto elem_re = store.get_unchecked(10);
  auto elem_re_2 = store.get_unchecked_const(10);
  auto comp_0 = store.get_component<0>(10);
  auto comp_0_re = store.get_component_unchecked<0>(10);
  const auto comp_0_re_2 = store.get_component_unchecked_const<0>(10);
}

int main() {
  Kokkos::initialize();
  type_1();
  type_2();
  Kokkos::finalize();
}
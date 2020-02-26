#include "math/prelude.hpp"
#include "storage/prelude.hpp"

using namespace storage;

using namespace math;

void type_1() {
  Storage<Kokkos::Serial, Kokkos::HostSpace, 4, int, int, float> store;
}

void type_2() {
  Storage<Kokkos::Serial, Kokkos::HostSpace, 4, Vector<float, 3>> store;
}

void type_3() {
  Storage<Kokkos::Serial, Kokkos::HostSpace, 4, Vector<float, 3>, Matrix<float, 4>> store;
}

int main() {
  Kokkos::initialize();
  type_1();
  type_2();
  type_3();
  Kokkos::finalize();
}
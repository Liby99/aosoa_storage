#include "storage/prelude.hpp"
#include "math/prelude.hpp"

using namespace storage;

using namespace math;

void type_1() {
  Storage<Kokkos::Serial, Kokkos::HostSpace, 4, int, int, float> store;
}

void type_2() {
  Storage<Kokkos::Serial, Kokkos::HostSpace, 4, Vector<float, 3>> store;
}

int main() {
  Kokkos::initialize();
  type_1();
  Kokkos::finalize();
}
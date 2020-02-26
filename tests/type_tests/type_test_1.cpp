#include "aosoa_storage/storage.hpp"

using namespace AosoaStorage;

void type_1() {
  Storage<Kokkos::Serial, Kokkos::HostSpace, 4, int, int, float> store;
}

int main() {
  Kokkos::initialize();
  type_1();
  Kokkos::finalize();
}
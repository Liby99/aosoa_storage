#include <assert.h>
#include <iostream>
#include <math/prelude.hpp>
#include <storage/prelude.hpp>

using namespace storage;
using namespace math;

using XVM = FullStorage<Config, Vector3f, Vector3f, float>;

void run() {
  XVM xvm;

  auto r1 = xvm.fill(5, Vector3f(1.0), Vector3f(1.0), 0.1);

  assert(xvm.size() == 5);
  assert(r1.start == 0);
  assert(r1.amount == 5);

  auto r2 = xvm.fill(5, Vector3f(2.0), Vector3f(2.0), 0.2);

  assert(xvm.size() == 10);
  assert(r2.start == 5);
  assert(r2.amount == 5);

  xvm.push();
}

int main() {
  Kokkos::initialize();
  run();
  Kokkos::finalize();
}
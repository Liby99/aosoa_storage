#include "../common/config.hpp"
#include <assert.h>
#include <iostream>
#include <math/prelude.hpp>
#include <storage/prelude.hpp>

using namespace storage;
using namespace storage_math;

using XVM = FullStorage<Config, Vector3f, Vector3f, float>;

void run() {
  XVM xvm;

  // xvm.size = 1024;

  // xvm.each([](typename XVM::HostHandle &handle) {
  //   handle.template set<0>(Vector3f(handle.i));
  //   handle.template set<1>(Vector3f());
  //   handle.template set<2>(0);
  // });

  // std::cout << "hahahah" << std::endl;

  // auto r1 = xvm.fill(10, Vector3f(1.0), Vector3f(1.0), 0.1);

  // assert(xvm.size == 5);
  // assert(r1.start == 0);
  // assert(r1.amount == 5);

  // std::cout << "hahahah" << std::endl;

  // auto r2 = xvm.fill(5, Vector3f(2.0), Vector3f(2.0), 0.2);

  // assert(xvm.size == 10);
  // assert(r2.start == 5);
  // assert(r2.amount == 5);

  // std::cout << "hahahah" << std::endl;

  // for (int i = 0; i < xvm.size; i++) {
  //   auto x = xvm.template get<0>(i);
  //   std::cout << i << ": " << x.x << " " << x.y << " " << x.z << std::endl;
  // }

  xvm.push();

  // std::cout << "hahahah" << std::endl;

  // xvm.par_each(KOKKOS_LAMBDA(const XVM::Handle &handle) {
  //   Vector3f x = handle.template get<0>();
  //   Vector3f v = handle.template get<1>();
  //   x += v;
  //   handle.template set<0>(x);
  // });

  // std::cout << "hahahah" << std::endl;

  // xvm.pull();

  // for (int i = 0; i < xvm.size; i++) {
  //   auto x = xvm.template get<0>(i);
  //   std::cout << i << ": " << x.x << " " << x.y << " " << x.z << std::endl;
  // }
}

int main() {
  Kokkos::initialize();
  run();
  Kokkos::finalize();
}
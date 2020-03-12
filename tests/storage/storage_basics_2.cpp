#include <storage/prelude.hpp>
#include <math/prelude.hpp>
#include "../common/grid_iterator.hpp"

using namespace storage;
using namespace math;

using XVM = FullStorage<Config, Vector3f, Vector3f, float>;

void run() {
  XVM xvm;

  auto iter = GridIterator<3>::range(100, 100, 100);
  xvm.fill_with_iter(iter, [](Vector<int, 3> node, typename XVM::HostHandle &handle) {
    handle.template set<0>(node.template cast<float>());
    handle.template set<1>(Vector3f(0.0, 10.0, 0.0));
    handle.template set<2>(0.01);
  });

  xvm.push();

  for (int i = 0; i < 100; i++) {
    xvm.par_each(KOKKOS_LAMBDA(typename XVM::DeviceHandle &handle) {
      auto x = handle.template get<0>();
      auto v = handle.template get<1>();
      handle.template set<0>(x + v);
    });
  }

  xvm.pull();
}

int main() {
  Kokkos::initialize();
  run();
  Kokkos::finalize();
}
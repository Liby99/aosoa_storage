#include <vector>
#include <cstdlib>
#include <storage/prelude.hpp>
#include <math/prelude.hpp>
#include "../common/config.hpp"
#include "../common/utils/grid_iterator.hpp"

using namespace storage;
using namespace math;

using XVM = FullStorage<Config, Vector3f, Vector3f, float>;

template <class T>
float random() {
  return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
}

void run() {
  XVM xvm;

  std::vector<Vector3f> positions(1000000);
  for (int i = 0; i < 1000000; i++) {
    positions.push_back(Vector3f(random<float>(), random<float>(), random<float>()));
  }

  xvm.fill_with_iter(positions, [](auto pos, typename XVM::HostHandle &handle) {
    handle.template set<0>(pos);
  });

  xvm.template fill<1>(Vector3f(0.0, 1.0, 0.0));
  xvm.template fill<2>(0.01);

  xvm.push();

  for (int i = 0; i < 10000; i++) {
    std::cout << "Frame " << i << "\r" << std::flush;
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
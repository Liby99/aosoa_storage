#include "../common/grid_iterator.hpp"
#include <cstdlib>
#include <math/prelude.hpp>
#include <storage/prelude.hpp>
#include <vector>

using namespace storage;
using namespace math;

using XVM = FullStorage<Config, Vector3f, Vector3f, float>;

void run() {
  XVM xvm;

  xvm.fill(10, Vector3f(), Vector3f(0.0, 1.0, 0.0), 0.01);

  xvm.push();

  for (int i = 0; i < 10000; i++) {
    std::cout << "Frame " << i << "\r" << std::flush;
    auto slice_x = Cabana::slice<0>(xvm.device_data);
    auto slice_v = Cabana::slice<1>(xvm.device_data);
    xvm.par_each(KOKKOS_LAMBDA(typename XVM::DeviceHandle & handle) {
      slice_x(handle.i, 0) += slice_v(handle.i, 0);
      slice_x(handle.i, 1) += slice_v(handle.i, 1);
      slice_x(handle.i, 2) += slice_v(handle.i, 2);
    });
  }

  xvm.pull();
}

int main() {
  Kokkos::initialize();
  run();
  Kokkos::finalize();
}
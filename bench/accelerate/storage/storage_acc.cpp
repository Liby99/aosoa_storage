#include <iostream>

#include "../../common/timer.hpp"
#include "./math/vector.hpp"
#include "./storage/storage.hpp"
#include "./utils/grid_iterator.hpp"

template <class T, int D>
void run() {
  Timer timer;

  int side = 100;
  int particle_amount = pow(side, D);

  T dt = 0.01;
  Vector<T, D> gravity;
  gravity.y = -9.8;
  Vector<int, D> size(side);

  using XVM = Storage<Vector<T, D>, Vector<T, D>, T>;
  XVM xvm(particle_amount);

  xvm.insert_iter(GridIterator<D>::range(size), [](auto node, auto &handle) {
    handle.template store<0>(node.template cast<T>());
    handle.template store<1>(Vector<T, D>(0));
    handle.template store<2>(0.01);
  });

  for (int frame = 0; frame < 100; frame++) {
    std::cout << "Frame " << frame << "\r" << std::flush;
    xvm.simd_par_each(KOKKOS_LAMBDA(auto &handle) {
      Vector<T, D> x = handle.template fetch<0>();
      Vector<T, D> v = handle.template fetch<1>();
      v += gravity * dt;
      x += v * dt;
      handle.template store<0>(x);
      handle.template store<1>(v);
    });
  }

  std::cout << "\nFinished in " << timer.elapsed<Timer::Milliseconds>() << " ms" << std::endl;
}

int main() {
  Kokkos::initialize();
  std::cout << "float, 2" << std::endl;
  run<float, 2>();
  std::cout << "double, 2" << std::endl;
  run<double, 2>();
  std::cout << "float, 3" << std::endl;
  run<float, 3>();
  std::cout << "double, 3" << std::endl;
  run<double, 3>();
  Kokkos::finalize();
}
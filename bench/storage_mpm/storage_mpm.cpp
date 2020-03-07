#include <stdio.h>

#include "./storage.hpp"
#include "./math/vector.hpp"
#include "./utils/grid_iterator.hpp"

template <class T, int D>
void run() {
  const int particle_amount = 1000000;

  T dt = 0.01;

  using XVM = Storage<Vector<T, D>, Vector<T, D>, T>;
  XVM xvm(particle_amount);

  xvm.insert_iter(GridIterator<2>::range(100, 100), [](auto node, auto &handle) {
    handle.template store<0>(Vector<T, D>(std::get<0>(node), std::get<1>(node)));
    handle.template store<1>(Vector<T, D>(0, -9.8));
    handle.template store<2>(0.01);
  });

  xvm.simd_par_each(KOKKOS_LAMBDA(auto &handle) {
    auto x = handle.template fetch<0>();
    auto v = handle.template fetch<1>();
    x += v * dt;
    handle.template store<0>(x);
  });

  xvm.each([](auto &handle) {
    auto x = handle.template fetch<0>();
    printf("x.x: %f, x.y: %f\n", x.x, x.y);
  });
}

int main() {
  Kokkos::initialize();
  run<float, 2>();
  Kokkos::finalize();
}
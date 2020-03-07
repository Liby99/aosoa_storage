#include <stdio.h>

#include "./storage/storage.hpp"
#include "./math/vector.hpp"
#include "./utils/grid_iterator.hpp"

template <class T, int D>
void run() {
  int particle_amount = 10000;

  T dt = 0.01;
  Vector<T, D> gravity;
  gravity.y = -9.8;
  Vector<int, D> size(1);

  using XVM = Storage<Vector<T, D>, Vector<T, D>, T>;
  XVM xvm(particle_amount);

  xvm.insert_iter(GridIterator<D>::range(size), [](auto node, auto &handle) {
    handle.template store<0>(Vector<T, D>(std::get<0>(node), std::get<1>(node)));
    handle.template store<1>(Vector<T, D>(0, 0));
    handle.template store<2>(0.01);
  });

  xvm.simd_par_each(KOKKOS_LAMBDA(auto &handle) {
    auto x = handle.template fetch<0>();
    auto v = handle.template fetch<1>();
    v -= gravity * dt;
    x += v * dt;
    handle.template store<0>(x);
    handle.template store<1>(v);
  });
}

int main() {
  Kokkos::initialize();
  run<float, 2>();
  Kokkos::finalize();
}
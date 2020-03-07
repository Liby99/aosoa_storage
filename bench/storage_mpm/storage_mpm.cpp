#include <stdio.h>

#include "./storage.hpp"
#include "./math/vector.hpp"
#include "./utils/grid_iterator.hpp"

template <class T, int D>
void run() {
  const int particle_amount = 1000000;

  using XVM = Storage<Vector<T, D>, Vector<T, D>, T>;
  XVM xvm(particle_amount);

  xvm.insert_iter(GridIterator<2>::range(100, 100), [](auto node, auto &handle) {
    handle.template store<0>(Vector<T, D>(std::get<0>(node), std::get<1>(node)));
    handle.template store<1>(Vector<T, D>(0));
    handle.template store<2>(0.01);
  });
}

int main() {
  Kokkos::initialize();
  run<float, 2>();
  Kokkos::finalize();
}
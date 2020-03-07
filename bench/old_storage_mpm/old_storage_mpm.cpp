#include <stdio.h>

#include "./config.hpp"
#include "./math/prelude.hpp"
#include "./storage/prelude.hpp"

using namespace math;

using namespace storage;

template <typename... Types>
using BaseStorage = Storage<ExecutionSpace, MemorySpace, BIN_SIZE, Types...>;

template <class T, int D>
using XVM = BaseStorage<Vector<T, D>, Vector<T, D>, T>;

template <class T, int D>
struct Initializer {
  static void initialize(XVM<T, D> &xvm, int side) {}
};

template <class T>
struct Initializer<T, 2> {
  static void initialize(XVM<T, 2> &xvm, int side) {
    for (int i = 0; i < side; i++) {
      for (int j = 0; j < side; j++) {
        xvm.append(Vector<T, 2>(i, j), Vector<T, 2>(), 0.01);
      }
    }
  }
};

template <class T>
struct Initializer<T, 3> {
  static void initialize(XVM<T, 3> &xvm, int side) {
    for (int i = 0; i < side; i++) {
      for (int j = 0; j < side; j++) {
        for (int k = 0; k < side; k++) {
          xvm.append(Vector<T, 3>(i, j, k), Vector<T, 3>(), 0.01);
        }
      }
    }
  }
};

template <class T, int D>
void run() {
  int side = 100;
  int particle_amount = pow(side, D);

  T dt = 0.01;
  Vector<T, D> gravity;
  gravity.y = -9.8;

  XVM<T, D> xvm(particle_amount);

  Initializer<T, D>::initialize(xvm, side);

  for (int frame = 0; frame < 100; frame++) {
    printf("Frame %d\r", frame);
    fflush(stdout);
    xvm.par_each(KOKKOS_LAMBDA(int, auto data) {
      auto &[x, v, m] = data;
      v -= gravity * dt;
      x += v * dt;
    });
  }
}

int main() {
  Kokkos::initialize();
  run<float, 2>();
  run<float, 3>();
  Kokkos::finalize();
}
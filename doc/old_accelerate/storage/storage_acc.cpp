#include <iostream>

#ifdef STORAGE_ENABLED_CUDA
#define DeviceFlag __host__ __device__
#else
#define DeviceFlag
#endif

#include "../../common/timer.hpp"
#include "./math/vector.hpp"
#include "./storage/storage.hpp"
#include "./utils/grid_iterator.hpp"

template <class T, int D>
using XVM = Storage<Vector<T, D>, Vector<T, D>, T>;

template <class T, int D>
struct Step {
  static void step(XVM<T, D> &xvm, T dt, Vector<T, D> &gravity) {
    xvm.simd_par_each(KOKKOS_LAMBDA(typename XVM<T, D>::SimdHandle &handle) {
      Vector<T, D> x = handle.template fetch<0>();
      Vector<T, D> v = handle.template fetch<1>();
      v += gravity * dt;
      x += v * dt;
      handle.template store<0>(x);
      handle.template store<1>(v);
    });
  }
};

template <class T, int D>
struct Step2 {
  static void step(XVM<T, D> &xvm, T dt, Vector<T, D> &gravity) {
    Vector<T, D> dv = gravity * dt;
    auto slice_pos = xvm.template slice<0>();
    auto slice_vel = xvm.template slice<1>();
    xvm.simd_par_each(KOKKOS_LAMBDA(typename XVM<T, D>::SimdHandle &handle) {
      for (int i = 0; i < D; i++) {
        slice_vel.access(handle.s, handle.a, i) += dv(i);
        slice_pos.access(handle.s, handle.a, i) +=
            slice_vel.access(handle.s, handle.a, i) * dt;
      }
    });
  }
};

template <class T, int D>
struct Step3 {
  static void step(XVM<T, D> &xvm, T dt, Vector<T, D> &gravity) {
    xvm.par_each(KOKKOS_LAMBDA(typename XVM<T, D>::Handle &handle) {
      Vector<T, D> x = handle.template fetch<0>();
      Vector<T, D> v = handle.template fetch<1>();
      v += gravity * dt;
      x += v * dt;
      handle.template store<0>(x);
      handle.template store<1>(v);
    });
  }
};

template <class T, int D>
struct Step4 {
  static void step(XVM<T, D> &xvm, T dt, Vector<T, D> &gravity) {
    using Handle = typename XVM<T, D>::Handle;
    Vector<T, D> dv = gravity * dt;
    auto slice_pos = xvm.template slice<0>();
    auto slice_vel = xvm.template slice<1>();
    xvm.par_each(KOKKOS_LAMBDA(Handle &handle) {
      // for (int i = 0; i < D; i++) {
      //   slice_vel(handle.i, i) += dv(i);
      //   slice_pos(handle.i, i) += slice_vel(handle.i, i) * dt;
      // }
    });
  }
};

template <template <class, int> class S, class T, int D>
void run() {
  Timer timer;

  int side = 100;
  int particle_amount = pow(side, D);

  T dt = 0.01;
  Vector<T, D> gravity;
  gravity.y = -9.8;
  Vector<int, D> size(side);

  XVM<T, D> xvm(particle_amount);
  xvm.size = particle_amount;

  xvm.insert_iter(GridIterator<D>::range(size), [](Vector<int, D> node, auto &handle) {
    handle.template store<0>(node.template cast<T>());
    handle.template store<1>(Vector<T, D>(0));
    handle.template store<2>(0.01);
  });

  for (int frame = 0; frame < 1000; frame++) {
    std::cout << "Frame " << frame << "\r" << std::flush;
    S<T, D>::step(xvm, dt, gravity);
  }

  std::cout << "\nFinished in " << timer.elapsed<Timer::Milliseconds>() << " ms"
            << std::endl;
}

int main() {
  Kokkos::initialize();
  // std::cout << "Step, float, 2" << std::endl;
  // run<Step, float, 2>();
  // std::cout << "Step, double, 2" << std::endl;
  // run<Step, double, 2>();
  // std::cout << "Step, float, 3" << std::endl;
  // run<Step, float, 3>();
  // std::cout << "Step, double, 3" << std::endl;
  // run<Step, double, 3>();
  // std::cout << "Step2, float, 2" << std::endl;
  // run<Step2, float, 2>();
  // std::cout << "Step2, double, 2" << std::endl;
  // run<Step2, double, 2>();
  // std::cout << "Step2, float, 3" << std::endl;
  // run<Step2, float, 3>();
  // std::cout << "Step2, double, 3" << std::endl;
  // run<Step2, double, 3>();
  // std::cout << "Step3, float, 2" << std::endl;
  // run<Step3, float, 2>();
  std::cout << "Step3, double, 2" << std::endl;
  run<Step3, double, 2>();
  // std::cout << "Step3, float, 3" << std::endl;
  // run<Step3, float, 3>();
  // std::cout << "Step3, double, 3" << std::endl;
  // run<Step3, double, 3>();
  // std::cout << "Step4, float, 2" << std::endl;
  // run<Step4, float, 2>();
  // std::cout << "Step4, double, 2" << std::endl;
  // run<Step4, double, 2>();
  // std::cout << "Step4, float, 3" << std::endl;
  // run<Step4, float, 3>();
  // std::cout << "Step4, double, 3" << std::endl;
  // run<Step4, double, 3>();
  Kokkos::finalize();
}
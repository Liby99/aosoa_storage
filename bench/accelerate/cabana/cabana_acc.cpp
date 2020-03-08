#include <random>

#include "../../common/timer.hpp"
#include "./storages.hpp"
#include "./vector.hpp"

template <class T>
T random() {
  return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
}

template <class T, int D>
void initialize(Particles<T, D> &particles, int start, int amount) {
  auto slice_pos = Cabana::slice<0>(particles);
  auto slice_vel = Cabana::slice<1>(particles);
  auto slice_mass = Cabana::slice<2>(particles);

  auto kernel = KOKKOS_LAMBDA(const int s, const int a) {
    // Set position
    for (int j = 0; j < D; j++) {
      slice_pos.access(s, a, j) = 0.0;
    }

    // Set velocity
    for (int j = 0; j < D; j++) {
      slice_vel.access(s, a, j) = 0.0;
    }

    // Set mass
    slice_mass.access(s, a) = 0.01;
  };

  Cabana::SimdPolicy<BIN_SIZE, ExecutionSpace> simd_policy(start, amount);
  Cabana::simd_parallel_for(simd_policy, kernel, "initialize");
}

template <class T, int D>
struct Step {
  static void step(Particles<T, D> &particles, T dt, int start, int amount) {
    T gravity[D];
    for (int i = 0; i < D; i++) {
      gravity[i] = 0.0;
    }
    gravity[1] = -9.8;

    auto slice_pos = Cabana::slice<0>(particles);
    auto slice_vel = Cabana::slice<1>(particles);
    auto slice_mass = Cabana::slice<2>(particles);

    auto kernel = KOKKOS_LAMBDA(const int s, const int a) {

      for (int j = 0; j < D; j++) {
        slice_vel.access(s, a, j) += gravity[j] * dt;
      }

      // Set position
      for (int j = 0; j < D; j++) {
        slice_pos.access(s, a, j) += slice_vel.access(s, a, j) * dt;
      }
    };

    Cabana::SimdPolicy<BIN_SIZE, ExecutionSpace> simd_policy(start, amount);
    Cabana::simd_parallel_for(simd_policy, kernel, "step");
  }
};

template <class T, int D>
struct Step2 {
  static void step(Particles<T, D> &particles, T dt, int start, int amount);
};

template <class T>
struct Step2<T, 2> {
  static void step(Particles<T, 2> &particles, T dt, int start, int amount) {
    Vector<T, 2> gravity(0, -9.8);

    auto slice_pos = Cabana::slice<0>(particles);
    auto slice_vel = Cabana::slice<1>(particles);
    auto slice_mass = Cabana::slice<2>(particles);

    auto kernel = KOKKOS_LAMBDA(const int s, const int a) {
      auto x =
          Vector<T, 2>(slice_pos.access(s, a, 0), slice_pos.access(s, a, 1));
      auto v =
          Vector<T, 2>(slice_vel.access(s, a, 0), slice_vel.access(s, a, 1));

      v += gravity * dt;
      x += v * dt;

      slice_vel.access(s, a, 0) = v.x;
      slice_vel.access(s, a, 1) = v.y;

      slice_pos.access(s, a, 0) = x.x;
      slice_pos.access(s, a, 1) = x.y;
    };

    Cabana::SimdPolicy<BIN_SIZE, ExecutionSpace> simd_policy(start, amount);
    Cabana::simd_parallel_for(simd_policy, kernel, "step");
  }
};

template <class T>
struct Step2<T, 3> {
  static void step(Particles<T, 3> &particles, T dt, int start, int amount) {
    Vector<T, 3> gravity(0, -9.8, 0);

    auto slice_pos = Cabana::slice<0>(particles);
    auto slice_vel = Cabana::slice<1>(particles);
    auto slice_mass = Cabana::slice<2>(particles);

    auto kernel = KOKKOS_LAMBDA(const int s, const int a) {
      auto x =
          Vector<T, 3>(slice_pos.access(s, a, 0), slice_pos.access(s, a, 1),
                       slice_pos.access(s, a, 2));
      auto v =
          Vector<T, 3>(slice_vel.access(s, a, 0), slice_vel.access(s, a, 1),
                       slice_vel.access(s, a, 2));

      v += gravity * dt;
      x += v * dt;

      slice_vel.access(s, a, 0) = v.x;
      slice_vel.access(s, a, 1) = v.y;
      slice_vel.access(s, a, 2) = v.z;

      slice_pos.access(s, a, 0) = x.x;
      slice_pos.access(s, a, 1) = x.y;
      slice_pos.access(s, a, 2) = x.z;
    };

    Cabana::SimdPolicy<BIN_SIZE, ExecutionSpace> simd_policy(start, amount);
    Cabana::simd_parallel_for(simd_policy, kernel, "step");
  }
};

template <class T, int D>
struct Step3 {
  static void step(Particles<T, D> &particles, T dt, int start, int amount) {
    T gravity[D];
    for (int i = 0; i < D; i++) {
      gravity[i] = 0.0;
    }
    gravity[1] = -9.8;

    auto slice_pos = Cabana::slice<0>(particles);
    auto slice_vel = Cabana::slice<1>(particles);
    auto slice_mass = Cabana::slice<2>(particles);

    auto kernel = KOKKOS_LAMBDA(const int i) {

      for (int j = 0; j < D; j++) {
        slice_vel(i, j) += gravity[j] * dt;
      }

      // Set position
      for (int j = 0; j < D; j++) {
        slice_pos(i, j) += slice_vel(i, j) * dt;
      }
    };

    Kokkos::RangePolicy<ExecutionSpace> linear_policy(start, amount);
    Kokkos::parallel_for(linear_policy, kernel, "step");
  }
};

template <template <class, int> class S, class T, int D>
void run() {
  Timer timer;

  const int side = 100;
  const int particle_count = pow(side, D);

  T dt = 0.01;

  // Initialize storages
  Particles<T, D> particles("particles", particle_count);

  // Initialize particles
  initialize<T, D>(particles, 0, particle_count);

  for (int frame = 0; frame < 100; frame++) {
    std::cout << "Frame " << frame << "\r" << std::flush;
    S<T, D>::step(particles, dt, 0, particle_count);
  }

  std::cout << "\nFinished in " << timer.elapsed<Timer::Milliseconds>() << " ms"
            << std::endl;
}

int main() {
  Kokkos::initialize();
  std::cout << "Step, float, 2" << std::endl;
  run<Step, float, 2>();
  std::cout << "Step, double, 2" << std::endl;
  run<Step, double, 2>();
  std::cout << "Step, float, 3" << std::endl;
  run<Step, float, 3>();
  std::cout << "Step, double, 3" << std::endl;
  run<Step, double, 3>();
  std::cout << "Step2, float, 2" << std::endl;
  run<Step2, float, 2>();
  std::cout << "Step2, double, 2" << std::endl;
  run<Step2, double, 2>();
  std::cout << "Step2, float, 3" << std::endl;
  run<Step2, float, 3>();
  std::cout << "Step2, double, 3" << std::endl;
  run<Step2, double, 3>();
  std::cout << "Step3, float, 2" << std::endl;
  run<Step3, float, 2>();
  std::cout << "Step3, double, 2" << std::endl;
  run<Step3, double, 2>();
  std::cout << "Step3, float, 3" << std::endl;
  run<Step3, float, 3>();
  std::cout << "Step3, double, 3" << std::endl;
  run<Step3, double, 3>();
  Kokkos::finalize();
}
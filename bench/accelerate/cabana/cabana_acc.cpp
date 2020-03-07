#include <random>

#include "../../common/timer.hpp"
#include "./storages.hpp"

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
void step(Particles<T, D> &particles, T dt, int start, int amount) {
  auto slice_pos = Cabana::slice<0>(particles);
  auto slice_vel = Cabana::slice<1>(particles);
  auto slice_mass = Cabana::slice<2>(particles);

  auto kernel = KOKKOS_LAMBDA(const int s, const int a) {
    slice_vel.access(s, a, 1) -= dt * 9.8;

    // Set position
    for (int j = 0; j < D; j++) {
      slice_pos.access(s, a, j) += slice_vel.access(s, a, j) * dt;
    }
  };

  Cabana::SimdPolicy<BIN_SIZE, ExecutionSpace> simd_policy(start, amount);
  Cabana::simd_parallel_for(simd_policy, kernel, "step");
}

template <class T, int D>
void run() {
  std::cout << "Running dimension " << D << std::endl;
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
    step<T, D>(particles, dt, 0, particle_count);
  }

  std::cout << "\nFinished in " << timer.elapsed<Timer::Seconds>() << " seconds" << std::endl;
}

int main() {
  Kokkos::initialize();
  run<float, 2>();
  run<float, 3>();
  Kokkos::finalize();
}
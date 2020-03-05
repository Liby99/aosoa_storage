#include "./kernels/initialize_particles.hpp"
#include "./storages.hpp"
#include <random>

template <class T>
T random() {
  return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
}

template <class T, int D>
void initialize_particles(Particles<T, D> &particles, int start, int amount) {
  auto slice_pos = Cabana::slice<0>(particles);
  auto slice_vel = Cabana::slice<1>(particles);
  auto slice_vel_grad = Cabana::slice<2>(particles);
  auto slice_affine = Cabana::slice<3>(particles);
  auto slice_mass = Cabana::slice<4>(particles);

  auto kernel = KOKKOS_LAMBDA(const int s, const int a) {
    // Set position
    for (int j = 0; j < D; j++) {
      slice_pos.access(s, a, j) = 0.0;
    }

    // Set velocity
    for (int j = 0; j < D; j++) {
      slice_vel.access(s, a, j) = 0.0;
    }

    // Set velocity gradient
    for (int j = 0; j < D; j++) {
      for (int k = 0; k < D; k++) {
        slice_vel_grad.access(s, a, j, k) = 0.0;
      }
    }

    // Set affine matrix
    for (int j = 0; j < D; j++) {
      for (int k = 0; k < D; k++) {
        slice_affine.access(s, a, j, k) = 0.0;
      }
    }

    // Set mass
    slice_mass.access(s, a) = 0.01;
  };

  Cabana::SimdPolicy<BIN_SIZE, ExecutionSpace> simd_policy(start, amount);
  Cabana::simd_parallel_for(simd_policy, kernel, "initialize_particles");
}

template <class T, int D>
void run() {
  const int particle_count = 100000000;
  Particles<T, D> particles("particles", particle_count);
  Stresses<T, D> stresses("stresses", particle_count);

  // Initialize particles
  initialize_particles<T, D>(particles, 0, particle_count);
}

int main() {
  Kokkos::initialize();
  run<float, 2>();
  Kokkos::finalize();
}
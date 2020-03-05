#include <random>
#include "./storages.hpp"
#include "./kernels/initialize_particles.hpp"

template <class T>
T random() {
  return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
}

template <class T, int D>
void initialize_particles(Particles<T, D> &particles, int start, int amount) {
  auto kernel = InitializeParticles<T, D>(particles);
  Kokkos::RangePolicy<ExecutionSpace> linear_policy(start, amount);
  Kokkos::parallel_for(linear_policy, kernel, "initialize_particles");
  Kokkos::fence();
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
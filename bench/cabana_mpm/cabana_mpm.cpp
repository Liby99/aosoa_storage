#include <random>
#include "../config.hpp"

// Position, Velocity, Gradient Velocity, Affine Matrix (C), Mass
template <class T, int D>
using ParticleTypes = Cabana::MemberTypes<T[D], T[D], T[D][D], T[D][D], T>;

template <class T, int D>
using Particles = Cabana::AoSoA<ParticleTypes<T, D>, KokkosDevice, BIN_SIZE>;

// Ks, Ls
template <class T, int D>
using StressTypes = Cabana::MemberTypes<T[D][D], T[D]>;

template <class T, int D>
using Stresses = Cabana::AoSoA<StressTypes<T, D>, KokkosDevice, BIN_SIZE>;

template <class T>
T random() {
  return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
}

template <class T, int D>
struct ParticleInitKernel {

  const Particles<T, D> &particles;
  decltype(Cabana::slice<0>(particles)) slice_pos;
  decltype(Cabana::slice<1>(particles)) slice_vel;
  decltype(Cabana::slice<2>(particles)) slice_vel_grad;
  decltype(Cabana::slice<3>(particles)) slice_affine;
  decltype(Cabana::slice<4>(particles)) slice_mass;

  using size_type = typename Particles<T, D>::size_type;

  ParticleInitKernel(const Particles<T, D> &particles) :
    particles(particles),
    slice_pos(Cabana::slice<0>(particles)),
    slice_vel(Cabana::slice<1>(particles)),
    slice_vel_grad(Cabana::slice<2>(particles)),
    slice_affine(Cabana::slice<3>(particles)),
    slice_mass(Cabana::slice<4>(particles)) {}

  KOKKOS_INLINE_FUNCTION void operator() (const size_type i) const {
    // Set position
    for (int j = 0; j < D; j++) {
      slice_pos(i, j) = 0.0;
    }

    // Set velocity
    for (int j = 0; j < D; j++) {
      slice_vel(i, j) = 0.0;
    }

    // Set velocity gradient
    for (int j = 0; j < D; j++) {
      for (int k = 0; k < D; k++) {
        slice_vel_grad(i, j, k) = 0.0;
      }
    }

    // Set affine matrix
    for (int j = 0; j < D; j++) {
      for (int k = 0; k < D; k++) {
        slice_affine(i, j, k) = 0.0;
      }
    }

    // Set mass
    slice_mass(i) = 0.01;
  }
};

template <class T, int D>
void initialize_particles(Particles<T, D> &particles, int start, int amount) {
  auto kernel = ParticleInitKernel<T, D>(particles);
  Kokkos::RangePolicy<ExecutionSpace> linear_policy(start, amount);
  Kokkos::parallel_for(linear_policy, kernel, "initialize_particles");
  Kokkos::fence();
}

template <class T, int D>
void run() {
  const int particle_count = 100000;
  Particles<T, D> particles("particles", particle_count);
  Stresses<T, D> stresses("stresses", particle_count);
  initialize_particles<T, D>(particles, 0, particle_count);
}

int main() {
  Kokkos::initialize();
  run<float, 2>();
  Kokkos::finalize();
}
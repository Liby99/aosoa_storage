#pragma once

#include "../storages.hpp"

template <class T, int D>
struct InitializeParticles {

  const Particles<T, D> &particles;
  decltype(Cabana::slice<0>(particles)) slice_pos;
  decltype(Cabana::slice<1>(particles)) slice_vel;
  decltype(Cabana::slice<2>(particles)) slice_vel_grad;
  decltype(Cabana::slice<3>(particles)) slice_affine;
  decltype(Cabana::slice<4>(particles)) slice_mass;

  using size_type = typename Particles<T, D>::size_type;

  InitializeParticles(const Particles<T, D> &particles) :
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
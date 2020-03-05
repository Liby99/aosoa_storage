#pragma once

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
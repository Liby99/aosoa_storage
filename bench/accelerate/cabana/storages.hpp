#pragma once

#include "../../common/config.hpp"

// Position, Velocity, Gradient Velocity, Affine Matrix (C), Mass
template <class T, int D>
using ParticleTypes = Cabana::MemberTypes<T[D], T[D], T>;

template <class T, int D>
using Particles = Cabana::AoSoA<ParticleTypes<T, D>, KokkosDevice, BIN_SIZE>;
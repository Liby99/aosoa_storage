#include <math/prelude.hpp>
#include <storage/prelude.hpp>

using namespace math;
using namespace storage;

template <class T, int D>
using MpmParticle = FullStorage<Config, Vector<T, D>, Vector<T, D>, Matrix<T, D>, Matrix<T, D>, T>;

template <class T, int D>
struct Fields<MpmParticle<T, D>> {
  enum Indices { X = 0, V, GRAD_V, C, M };
};

template <class T, int D>
using MpmStress = FullStorage<Config, Matrix<T, D>, Vector<T, D>>;

template <class T, int D>
struct Fields<MpmStress<T, D>> {
  enum Indices { KS = 0, LS };
};

template <class T, int D>
using FixedCorotated = RangedStorage<Config, Matrix<T, D>, T, T, T>;

template <class T, int D>
struct Fields<FixedCorotated<T, D>> {
  enum Indices { F = 0, VOL, LAMBDA, MU };
};

template <class T, int D>
void run() {
  MpmParticle<T, D> particles;
  MpmStress<T, D> stresses;
}

int main() {
  Kokkos::initialize();
  run<float, 2>();
  Kokkos::finalize();
}
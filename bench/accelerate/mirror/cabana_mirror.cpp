#include "../../common/config.hpp"

using particle_fields = Cabana::MemberTypes<float[3], float[3], int>;
using particle_list = Cabana::AoSoA<particle_fields,Kokkos::CudaSpace>;

void run() {
  particle_list particles("particles", 100);
  auto host_particles = Cabana::create_mirror_view_and_copy(Kokkos::HostSpace(), particles);

  auto slice_0 = Cabana::slice<0>(host_particles);
  auto slice_1 = Cabana::slice<1>(host_particles);
  auto slice_2 = Cabana::slice<2>(host_particles);
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 3; j++) {
      slice_0(i, j) = i;
      slice_1(i, j) = i;
    }
    slice_2(i) = i;
  }
}

int main() {
  Kokkos::initialize();
  run();
  Kokkos::finalize();
}
#include "../../common/config.hpp"

using particle_fields = Cabana::MemberTypes<float[3], float[3], float>;
using attr_fields = Cabana::MemberTypes<float[3]>;
using particle_list = Cabana::AoSoA<particle_fields, KokkosDevice>;
using attr_list = Cabana::AoSoA<attr_fields, KokkosDevice>;

void run() {
  particle_list particles("particles", 100);
  auto host_particles = Cabana::create_mirror_view_and_copy(Kokkos::HostSpace(), particles);

  attr_list attrs("attrs", 100);
  auto host_attrs = Cabana::create_mirror_view_and_copy(Kokkos::HostSpace(), attrs);

  {
    auto slice_0 = Cabana::slice<0>(host_particles);
    auto slice_1 = Cabana::slice<1>(host_particles);
    auto slice_2 = Cabana::slice<2>(host_particles);
    auto slice_3 = Cabana::slice<0>(host_attrs);

    auto slice_0_copy = slice_0;
    auto slice_1_copy = slice_1;
    auto slice_2_copy = slice_2;
    auto slice_3_copy = slice_3;

    for (int i = 0; i < 100; i++) {
      for (int j = 0; j < 3; j++) {
        slice_0_copy(i, j) = i;
        slice_1_copy(i, j) = 0.0;
        slice_3_copy(i, j) = 0.0;
      }
      slice_3_copy(i, 1) = 1.0;
      slice_2_copy(i) = 0.01;
    }
  }

  {
    Cabana::deep_copy(particles, host_particles);
    Cabana::deep_copy(attrs, host_attrs);
  }

  {
    auto device_slice_0 = Cabana::slice<0>(particles);
    auto device_slice_1 = Cabana::slice<1>(particles);
    auto device_slice_3 = Cabana::slice<0>(attrs);

    const auto device_slice_0_copy = device_slice_0;
    const auto device_slice_1_copy = device_slice_1;
    const auto device_slice_3_copy = device_slice_3;

    auto kernel = KOKKOS_LAMBDA(const int i) {
      for (int j = 0; j < 3; j++) {
        device_slice_1_copy(i, j) += device_slice_3_copy(i, j);
        device_slice_0_copy(i, j) += device_slice_1_copy(i, j);
      }
    };

    Kokkos::RangePolicy<ExecutionSpace> linear_policy(0, 100);
    for (int i = 0; i < 100; i++) {
      Kokkos::parallel_for(linear_policy, kernel, "step");
    }
  }

  {
    Cabana::deep_copy(host_particles, particles);
    Cabana::deep_copy(host_attrs, attrs);
  }

  {
    auto slice_0 = Cabana::slice<0>(host_particles);
    for (int i = 0; i < 100; i++) {
      std::cout << i << ": " << slice_0(i, 0) << ", " << slice_0(i, 1) << ", " << slice_0(i, 2)
                << std::endl;
    }
  }
}

int main() {
  Kokkos::initialize();
  run();
  Kokkos::finalize();
}
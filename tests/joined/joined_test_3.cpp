#include <math/prelude.hpp>
#include <storage/prelude.hpp>

using namespace storage;
using namespace math;

using Particles = FullStorage<Config, Vector3f, Vector3f, float>; // x v m
using Accelerations = FullStorage<Config, Vector3f>;              // a

void run() {
  Particles particles;
  Accelerations accelerations;

  particles.fill(5, Vector3f(), Vector3f(0.0, 10.0, 0.0), 0.01);
  accelerations.fill(5, Vector3f(0.0, -1.0, 0.0));

  particles.push();
  accelerations.push();

  std::cout << "hahahah 1" << std::endl;

  auto device_slice_x = Cabana::slice<0>(particles.device_data);

  particles.par_each(KOKKOS_LAMBDA(typename Particles::DeviceHandle & handle) {
    auto x = handle.template get<0>();
    auto v = handle.template get<1>();
    x.y += 1000.0;
    handle.template set<0>(x);
    // device_slice_x(handle.i, 1) += 1000.0;
  });

  Kokkos::fence();

  std::cout << "hahahah 2" << std::endl;

  particles.pull();

  particles.each([](typename Particles::HostHandle &handle) {
    auto x = handle.template get<0>();
    std::cout << handle.i << ": " << x.x << ", " << x.y << ", " << x.z << std::endl;
  });

  std::cout << "hahahah 3" << std::endl;

  for (int i = 0; i < 10; i++) {
    std::cout << "Frame " << i << std::endl;
    using Joined = decltype(particles.join(accelerations));

    auto device_slice_x = Cabana::slice<0>(particles.device_data);
    auto device_slice_v = Cabana::slice<1>(particles.device_data);

    particles.join(accelerations).par_each(KOKKOS_LAMBDA(typename Joined::DeviceHandle & handle) {
      // auto x = Vector3f(device_slice_x(handle.i, 0), device_slice_x(handle.i, 1),
      // device_slice_x(handle.i, 2)); x.y += 10.0;
      device_slice_x(handle.i, 1) += 10.0;
      // device_slice_x(handle.i, 0) = x.x;
      // device_slice_x(handle.i, 1) = x.y;
      // device_slice_x(handle.i, 2) = x.z;
      // handle.template set<0, 0>(x);
      // handle.template set<0, 1>(v);
    });
    Kokkos::fence();
  }

  std::cout << "HHHHHH" << std::endl;

  particles.pull();

  // particles.each([](auto &handle) {
  //   auto x = handle.template get<0>();
  //   std::cout << handle.i << ": " << x.x << ", " << x.y << ", " << x.z << std::endl;
  // });
}

int main() {
  Kokkos::initialize();
  run();
  Kokkos::finalize();
}
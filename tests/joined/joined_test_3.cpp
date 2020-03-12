#include <math/prelude.hpp>
#include <storage/prelude.hpp>

using namespace storage;
using namespace math;

using Particles = FullStorage<Config, Vector3f, Vector3f, float>; // x v m
using Accelerations = FullStorage<Config, Vector3f>;              // a

void run() {
  Particles particles;
  Accelerations accelerations;

  particles.fill(10000000, Vector3f(), Vector3f(0.0, 10.0, 0.0), 0.01);
  accelerations.fill(10000000, Vector3f(0.0, -1.0, 0.0));

  particles.push();
  accelerations.push();

  for (int i = 0; i < 10000; i++) {
    std::cout << "Frame " << i << "\r" << std::flush;
    using Joined = decltype(particles.join(accelerations));
    particles.join(accelerations).par_each(KOKKOS_LAMBDA(typename Joined::DeviceHandle & handle) {
      auto x = handle.template get<0, 0>();
      auto v = handle.template get<0, 1>();
      auto a = handle.template get<1, 0>();
      v += a;
      x += v;
      handle.template set<0, 0>(x);
      handle.template set<0, 1>(v);
    });
  }

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
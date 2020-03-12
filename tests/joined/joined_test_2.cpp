#include <math/prelude.hpp>
#include <storage/prelude.hpp>

using namespace storage;
using namespace math;

using Particles = FullStorage<Config, Vector3f, Vector3f, float>; // x v m
using Accelerations = FullStorage<Config, Vector3f>;              // a

void run() {
  Particles particles;
  Accelerations accelerations;

  particles.fill(5, Vector3f(), Vector3f(), 0.01);
  accelerations.fill(5, Vector3f(0.0, 1.0, 0.0));

  for (int i = 0; i < 100; i++) {
    particles.join(accelerations).each([](auto &handle) {
      auto x = handle.template get<0, 0>();
      auto v = handle.template get<0, 1>();
      auto a = handle.template get<1, 0>();
      v += a;
      x += v;
      handle.template set<0, 0>(x);
      handle.template set<0, 1>(v);
    });
  }

  particles.each([](auto &handle) {
    auto x = handle.template get<0>();
    std::cout << handle.i << ": " << x.x << ", " << x.y << ", " << x.z << std::endl;
  });
}

int main() {
  Kokkos::initialize();
  run();
  Kokkos::finalize();
}
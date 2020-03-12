#include <math/prelude.hpp>
#include <storage/prelude.hpp>

using namespace storage;
using namespace math;

using Particles = FullStorage<Config, Vector3f, Vector3f, float>; // a b c
using Stresses = FullStorage<Config, Vector3f, Vector3f>;         // d e
using Attr1 = RangedStorage<Config, Vector3f, float>;             // f g
using Attr2 = RangedStorage<Config, float, int>;                  // h i

void run() {
  Particles particles;
  auto r1 = particles.fill(10000, Vector3f(), Vector3f(), 0.01);
  auto r2 = particles.fill(10000, Vector3f(), Vector3f(), 0.01);

  std::cout << particles.size() << std::endl;

  Stresses stresses;
  stresses.fill(particles.size(), Vector3f(), Vector3f());

  Attr1 attr1;
  attr1.fill(r1, Vector3f(), 5.0);

  Attr2 attr2;
  attr2.fill(r2, 5.0, 100);

  particles.join(stresses).each([](auto &handle) {
    auto a = handle.template get<0, 0>();
    auto e = handle.template get<1, 1>();
    a += e;
    handle.template set<0, 0>(a);
  });
}

int main() {
  Kokkos::initialize();
  run();
  Kokkos::finalize();
}
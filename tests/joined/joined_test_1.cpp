#include <storage/prelude.hpp>
#include <math/prelude.hpp>

using namespace storage;
using namespace math;

using Particles = FullStorage<Config, Vector3f, Vector3f, float>;
using Stresses = FullStorage<Config, Vector3f, Vector3f>;
using Attr1 = RangedStorage<Config, Vector3f, float>;
using Attr2 = RangedStorage<Config, float, int>;

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

  auto j1 = particles.join(stresses);
  std::cout << "j1 ranges: " << j1.ranges().to_string() << std::endl;

  auto j2 = particles.join(attr1);
  std::cout << "j2 ranges: " << j2.ranges().to_string() << std::endl;

  auto j3 = particles.join(attr2);
  std::cout << "j3 ranges: " << j3.ranges().to_string() << std::endl;

  j1.each(3);
}

int main() {
  Kokkos::initialize();
  run();
  Kokkos::finalize();
}
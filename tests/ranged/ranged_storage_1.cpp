#include <storage/prelude.hpp>
#include "../common/grid_iterator.hpp"

using namespace storage;
using namespace math;

using XVM = FullStorage<Config, Vector3f, Vector3f, float>;

using Attr1 = RangedStorage<Config, Vector3f, float>;

using Attr2 = RangedStorage<Config, float, int>;

int main() {
  XVM xvm;
  Attr1 attr1;
  Attr2 attr2;

  auto iter_1 = GridIterator<3>::range(10, 10, 10);
  auto r1 = xvm.fill_with_iter(iter_1, [](Vector<int, 3> node, typename XVM::HostHandle &handle) {
    handle.template set<0>(node.template cast<float>());
  });

  attr1.fill(r1, Vector3f(), 5.0);

  auto iter_2 = GridIterator<3>::range(10, 10, 10);
  auto offset = Vector3f(20, 0, 0);
  auto r2 = xvm.fill_with_iter(iter_2, [&](Vector<int, 3> node, typename XVM::HostHandle &handle) {
    handle.template set<0>(node.template cast<float>() + offset);
  });

  xvm.template fill<1>(Vector3f(0.0, 1.0, 0.0));
  xvm.template fill<2>(0.01);

  attr2.fill(r2, 3.0, 100);
}
#include <storage/prelude.hpp>
#include <math/prelude.hpp>
#include "../common/config.hpp"

using namespace storage;
using namespace storage_math;

template <typename... Types>
using Full = FullStorage<ExecutionSpace, MemorySpace, BIN_SIZE, Types...>;

using XVM = Full<Vector3f, Vector3f, float>;

void run() {
  XVM xvm;
}

int main() {
  Kokkos::initialize();
  run();
  Kokkos::finalize();
}
#include <memory>
#include "math/prelude.hpp"
#include "storage/prelude.hpp"

using namespace storage;

using namespace math;

template <typename... Types>
using BaseStorage = Storage<Kokkos::OpenMP, Kokkos::HostSpace, 4, Types...>;

template <typename T, int dim>
using MyStorage = BaseStorage<T, Vector<T, dim>, Vector<T, dim>>;

template <std::size_t Offset, typename T, int dim>
struct storage::FieldsWithOffset<Offset, MyStorage<T, dim>> {
  enum Indices { M, V, X };
};

template <typename T, int dim>
struct Functor {
  using Storage = MyStorage<T, dim>;

  static std::unique_ptr<Storage> create() {
    return std::make_unique<Storage>();
  }
};

void test() {
  auto store = Functor<float, 3>::create();
}

int main() {
  Kokkos::initialize();
  test();
  Kokkos::finalize();
}
#include <assert.h>
#include <iostream>
#include <storage/prelude.hpp>

using namespace storage;

void test_1() {
  Ranges r1, r2, r3;

  r1.add(0, 100);
  r2.add(0, 40);
  r3.add(60, 20);
  r3.add(20, 20);

  // std::cout << "r1: " << r1.to_string() << std::endl;
  // std::cout << "r2: " << r2.to_string() << std::endl;
  // std::cout << "r3: " << r3.to_string() << std::endl;

  auto i12 = r1.intersect(r2);
  assert(i12.ranges[0].start == 0);
  assert(i12.ranges[0].end() == 40);
  // std::cout << "i12: " << i12.to_string() << std::endl;

  auto i23 = r2.intersect(r3);
  assert(i23.ranges[0].start == 20);
  assert(i23.ranges[0].end() == 40);
  // std::cout << "i23: " << i23.to_string() << std::endl;

  auto i13 = r1.intersect(r3);
  assert(i13.ranges[0].start == 20);
  assert(i13.ranges[0].end() == 40);
  assert(i13.ranges[1].start == 60);
  assert(i13.ranges[1].end() == 80);
  // std::cout << "i13: " << i13.to_string() << std::endl;

  auto i123 = r1.intersect(r2).intersect(r3);
  assert(i123.ranges[0].start == 20);
  assert(i123.ranges[0].end() == 40);
  // std::cout << "i123: " << i123.to_string() << std::endl;
}

int random(int max) {
  return rand() % max;
}

Ranges random_ranges() {
  Ranges ranges;
  int num_ranges = random(5);
  int curr_max = 0;
  for (int i = 0; i < num_ranges; i++) {
    int curr_start = curr_max + random(50);
    int curr_amount = random(100);
    ranges.add(curr_start, curr_amount);
    curr_max = curr_start + curr_amount;
  }
  return ranges;
}

void test_2() {
  for (int i = 0; i < 1000; i++) {
    Ranges r1 = random_ranges();
    Ranges r2 = random_ranges();
    Ranges itsct = r1.intersect(r2);
    // std::cout << "r1: " << r1.to_string() << std::endl;
    // std::cout << "r2: " << r2.to_string() << std::endl;
    // std::cout << "itsct: " << itsct.to_string() << std::endl;
    for (auto range : itsct) {
      for (int j = range.begin(); j < range.end(); j++) {
        assert(r1.contains(j));
        assert(r2.contains(j));
      }
    }
  }
}

void test_3() {
  RangesMap ranges_map;
  ranges_map.add(30, 200, 100);
  ranges_map.add(0, 30, 30);
}

int main() {
  test_1();
  test_2();
  test_3();
}
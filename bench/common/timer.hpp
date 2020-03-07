#pragma once

#include <chrono>

struct Timer {
  using Seconds = std::chrono::seconds;

  decltype(std::chrono::high_resolution_clock::now()) start;

  Timer() : start(std::chrono::high_resolution_clock::now()) {}

  template <typename T>
  long elapsed() {
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<T>(stop - start);
    return duration.count();
  }
};
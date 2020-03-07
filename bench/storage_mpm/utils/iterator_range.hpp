#pragma once

template <typename IteratorType>
struct IteratorRange {
  IteratorType begin_it, end_it;

  template <typename Container>
  IteratorRange(Container &&c) : begin_it(c.begin()), end_it(c.end()) {}

  IteratorRange(IteratorType begin_it, IteratorType end_it)
    : begin_it(std::move(begin_it)), end_it(std::move(end_it)) {}

  IteratorType begin() const { return begin_it; }

  IteratorType end() const { return end_it; }

  bool empty() const { return begin_it == end_it; }
};

template <class T>
IteratorRange<T> make_range(T x, T y) {
  return IteratorRange<T>(std::move(x), std::move(y));
}
#pragma once

namespace storage {
  struct Range {
    std::size_t start, amount;

    std::size_t end() const {
      return start + amount;
    }
  };

  // struct Ranges {
  //   std::vector<Range> ranges;

  //   void add(const Range &range) {
  //     Range &last_range = ranges[ranges.size() - 1];
  //     if (last_range.end() == range.start) {
  //       last_range.
  //     }
  //   }
  // };

  // struct Ranges {
  //   std::vector<std::tuple<std::size_t, Range>> ranges;

  //   void add(std::size_t local_start, const Range &range) {
  //     ranges.push_back(std::make_tuple(local_start, range));
  //   }


  // };
}
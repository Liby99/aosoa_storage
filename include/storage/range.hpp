#pragma once

namespace storage {
  struct Range {
    std::size_t start, amount;

    Range(std::size_t start, std::size_t amount)
        : start(start), amount(amount) {}

    Range() : Range(0, 0) {}

    inline bool intersects(const Range &other) const {
      return end() > other.start && start < other.end();
    }

    inline bool operator<= (const Range &other) const {
      return end() <= other.end();
    }

    inline std::size_t begin() const {
      return start;
    }

    inline std::size_t end() const {
      return start + amount;
    }

    inline std::size_t size() const {
      return amount;
    }

    inline bool contains(std::size_t i) const {
      return i >= begin() && i < end();
    }

    Range intersect(const Range &other) const {
      std::size_t res_start = std::max(start, other.start);
      std::size_t res_end = std::min(end(), other.end());
      if (res_start > res_end) {
        return Range();
      } else {
        return Range(res_start, res_end - res_start);
      }
    }

    std::string to_string() const {
      return "[" + std::to_string(start) + ", " + std::to_string(end()) + ")";
    }
  };

  struct Ranges {
    std::vector<Range> ranges;

    void add(const Range &range) {
      ranges.push_back(range);
      if (ranges.size() > 1) {
        int curr = ranges.size() - 2;
        while (curr >= 0 && range <= ranges[curr]) {
          std::swap(ranges[curr], ranges[curr + 1]);
          curr--;
        }
      }
    }

    void add(std::size_t start, std::size_t amount) {
      add(Range(start, amount));
    }

    auto begin() const {
      return ranges.begin();
    }

    auto end() const {
      return ranges.end();
    }

    bool contains(std::size_t i) const {
      for (auto range : ranges) {
        if (range.contains(i)) return true;
      }
      return false;
    }

    Ranges intersect(const Ranges &other) const {
      Ranges result;
      std::size_t i = 0, j = 0;
      while (i < ranges.size() && j < other.ranges.size()) {
        const Range &curr_i = ranges[i];
        const Range &curr_j = other.ranges[j];

        // Check if there's intersection. If there is, add it to the result
        if (curr_i.intersects(curr_j)) {
          result.add(curr_i.intersect(curr_j));
        }

        // Move forward
        if (curr_i <= curr_j) {
          i++;
        } else {
          j++;
        }
      }
      return result;
    }

    std::string to_string() const {
      std::string result = "[";
      for (int i = 0; i < ranges.size(); i++) {
        result += ranges[i].to_string();
        if (i != ranges.size() - 1) {
          result += ", ";
        }
      }
      result += "]";
      return result;
    }
  };

  struct RangesMap {
    Ranges globals;
    std::vector<std::size_t> locals;

    void add(std::size_t local, const Range &global) {
      locals.push_back(local);
      globals.add(global);
    }
  };
}
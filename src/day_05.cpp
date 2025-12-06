#include "shared/io.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

using Interval = std::pair<long long, long long>;

struct Input {
  std::vector<Interval> ranges;
  std::vector<long long> ids;
};

Input parse_input(const std::vector<std::string> &lines) {
  Input data;
  std::istringstream iss;

  for (const auto &line : lines) {
    if (line.empty())
      continue;

    // We can safely assume that ranges are positive, so the only
    // thing we need to check is if the line contains a dash
    if (line.find('-') != std::string::npos) {
      iss.clear();
      iss.str(line);

      long long start, end;
      char dash;
      if (iss >> start >> dash >> end) {
        data.ranges.push_back({start, end});
      }
    } else
      data.ids.push_back(std::stol(line));
  }

  return data;
}

std::vector<Interval> merge_intervals(std::vector<Interval> ranges) {
  if (ranges.empty())
    return {};

  std::sort(ranges.begin(), ranges.end());
  std::vector<Interval> merged;

  for (size_t i = 0; i < ranges.size(); i++) {
    auto &cur = merged.back();
    const auto &next = ranges[i];
    if (merged.empty() || cur.second < next.first - 1) {
      merged.push_back(next);
    } else {
      cur.second = std::max(cur.second, next.second);
    }
  }

  return merged;
}

void part1(const Input &data) {
  auto merged = merge_intervals(data.ranges);
  int ans = 0;

  for (const auto &[start, end] : data.ranges) {
    // Binary search for the fist id that starts after the query
    auto it = std::upper_bound(
        merged.begin(), merged.end(), start,
        [](long long val, const Interval &range) { return val < range.first; });
    if (it != merged.begin()) {
      auto prev = *std::prev(it);
      if (start <= prev.second) {
        ans++;
      }
    }
  }

  std::cout << ans << std::endl;
}

void part2(const Input &data) {
  auto merged = merge_intervals(data.ranges);
  long long ans = 0;

  for (auto &[start, end] : merged) {
    ans += end - start + 1;
  }

  std::cout << ans << std::endl;
}

int main() {
  auto lines = aoc::io::read_lines();
  auto data = parse_input(lines);

  part1(data);
  part2(data);

  return 0;
}

/** Notes
 * The input has this form: a list of fresh ingredient ID ranges, a blank
 line, and a list of available ingredient IDs.
 * The goal is to find the number of fresh IDs.
 *
 * Example:
 * 3-5
 * 10-14
 * 16-20
 * 12-18
 *
 * 1
 * 5
 * 8
 * 11
 * 17
 * 32
 *
 * In this case: 1 is not fresh (no range contains it), 5 is fresh (ranges are
 inclusive), 8 is not, 11 is, 17 is and 32 is not.
 * So we have 3 fresh IDs.
 *
 * Note that the ranges overlap. Maybe we can merge them to reduce the number
 of checks.
 * A bruteforce approach would be to check each ID against each range.
 *
 * TC: O(n) -> we use binary search to find the first range that contains the ID.
 * SC: O(n) -> we need to store the fresh ranges and the IDs to check.
 *
 * A first improvement could be to sort the ranges and merge them if they
 overlap.
 *
 * The second part just asks for the number of fresh IDs. Once merged the
 ranges, we can simply count the number of gaps between them.
 *
 * TC: O(n log n) -> sorting the ranges takes O(n log n) time.
 * SC: O(n) -> we need to store the fresh ranges and the IDs to check.
 */

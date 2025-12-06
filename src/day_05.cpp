#include "shared/io.hpp"
#include <iostream>
#include <sstream>
#include <vector>

void part1(const std::vector<std::string> &lines) {
  std::vector<std::pair<long long, long long>> fresh_ranges;
  std::vector<long long> to_check;

  std::istringstream iss;

  for (const auto &line : lines) {
    if (line.empty())
      continue;
    if (line.find('-') != std::string::npos) {
      iss.clear();
      iss.str(line);

      long long start, end;
      char dash;
      if (iss >> start >> dash >> end) {
        fresh_ranges.push_back({start, end});
      }
    }

    else
      to_check.push_back(std::stol(line));
  }

  // process the ranges
  std::sort(fresh_ranges.begin(), fresh_ranges.end());
  std::vector<std::pair<long long, long long>> merged_ranges;
  for (auto &[start, end] : fresh_ranges) {
    if (merged_ranges.empty() || merged_ranges.back().second < start - 1) {
      merged_ranges.push_back({start, end});
    } else {
      merged_ranges.back().second = std::max(merged_ranges.back().second, end);
    }
  }

  int ans = 0;
  for (auto &id : to_check) {
    for (auto &[start, end] : merged_ranges) {
      if (id >= start && id <= end) {
        ans++;
        break;
      }
    }
  }

  std::cout << ans << std::endl;
}

void part2(const std::vector<std::string> &lines) {
  std::vector<std::pair<long long, long long>> fresh_ranges;

  std::istringstream iss;

  for (const auto &line : lines) {
    if (line.empty())
      continue;
    if (line.find('-') != std::string::npos) {
      iss.clear();
      iss.str(line);

      long long start, end;
      char dash;
      if (iss >> start >> dash >> end) {
        fresh_ranges.push_back({start, end});
      }
    }
  }

  // process the ranges
  std::sort(fresh_ranges.begin(), fresh_ranges.end());
  std::vector<std::pair<long long, long long>> merged_ranges;
  for (auto &[start, end] : fresh_ranges) {
    if (merged_ranges.empty() || merged_ranges.back().second < start - 1) {
      merged_ranges.push_back({start, end});
    } else {
      merged_ranges.back().second = std::max(merged_ranges.back().second, end);
    }
  }

  long long ans = 0;

  for (auto &[start, end] : merged_ranges) {
    ans += end - start + 1;
  }

  std::cout << ans << std::endl;
}

int main() {
  auto lines = aoc::io::read_lines();

  // part1(lines);
  part2(lines);

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
 * TC: O(n * m) -> we have to test n values against m ranges.
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

 // TODO: Refactor this in a more professional way. DRY and refactored to separate concerns. Use a struct to represent the input
 // and extract the parsing logic from the main "solver" function. same thing with merge_intervals;

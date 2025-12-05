#include "shared/io.hpp"
#include <iostream>
#include <string_view>
#include <vector>

long long solve_line(const std::string_view line, size_t n) {
  std::string stack;
  stack.reserve(n);

  int len = line.length();
  for (int i = 0; i < len; i++) {
    char curr = line[i];
    // while non empty and curr > stack.back (top element) and we have more than
    // 12 digits
    while (!stack.empty() && stack.back() < curr &&
           (stack.size() - 1 + (len - i)) >= n) {
      stack.pop_back();
    }

    if (stack.size() < n)
      stack.push_back(curr);
  }
  return std::stoll(stack);
}

void solve(const std::vector<std::string> &lines) {
  long long ans = 0;

  for (auto &line : lines) {
    ans += solve_line(line, 12);
  }
  std::cout << ans << std::endl;
}

int main() {
  auto lines = aoc::io::read_lines();
  solve(lines);

  return 0;
}

/** Notes
 * The batteries are arranged into banks; each line of digits in the input
 * corresponds to a single bank of batteries.
 *
 * We need to turn on exactly two batteries.
 * The "power" of the batteries is formed by the digits turned on, e.g.
 * battery: [123456789] -> turned on: 4,5 -> power: 45 jolt (special unit for
 * this problem).
 *
 * We need to find the max amount a battery can produce.
 *
 * Bruteforce idea: for each line, test every possible ans and add the max
 *
 * TC: O(n^2) -> linear scan on every row
 * SC: O(1) -> no extra space required
 *
 * One possible improvement is to iterate from the largest possible digit down
 * to zero and look for valid pairs. This is a greedy approach that leads to a
 * reduction in overall time complexity.
 *
 * TC: O(n) -> now the bottleneck is the find operation
 * SC: O(1) -> no extra space required
 *
 * Part two requires switching to an arbitrary N (12).
 * A bruteforce approach is unlikely to be working (O(n^12)), so we need some
 * improvements.
 *
 * A monotonic decreasing stack could be useful? We just need to think about if
 * we have enough digits in the input.
 *
 * TC: O(n) -> same as above
 * SC: O(n) -> the stack grows like the string
 */

#include "shared/io.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Range {
  long long start;
  long long end;
};

int get_digit_count(long long number) {
  int digits = 0;
  do {
    number /= 10;
    digits++;
  } while (number);
  return digits;
}

bool is_invalid(long long number) {

  int digits = get_digit_count(number);

  for (int k = 1; k <= digits / 2; k++) {
    if (digits % k != 0)
      continue;
    long long multiplier = std::pow(10, k); // FIXME: could have performance issues

    // Construction
    long long seed = number % multiplier;
    long long candidate = 0;
    long long repetitions = digits / k;

    for (int i = 0; i < repetitions; i++) {
      candidate = (candidate * multiplier) + seed;
    }

    if (candidate == number)
      return true;
  }

  return false;
}

void solve(const std::vector<Range> &ranges) {
  long long ans = 0;
  for (const auto &r : ranges) {
    for (long long i = r.start; i <= r.end; i++)
      if (is_invalid(i))
        ans += i;
  }
  std::cout << ans << std::endl;
}

int main() {
  auto lines = aoc::io::read_lines();

  std::vector<Range> ranges;
  std::string line = lines[0];
  std::stringstream ss(line);
  std::string range;

  while (std::getline(ss, range, ',')) {
    ranges.push_back({std::stoll(range.substr(0, range.find('-'))),
                      std::stoll(range.substr(range.find('-') + 1))});
  }

  solve(ranges);

  return 0;
}

/** Notes
 *
 * We're given a set of ranges, like "11-22" and we need to find the invalid
 * ids. An id is invalid if it contains a repeated character, like in the range
 * "11-22", we have two invalid ids: 11 and 22. The solution is the sum of all
 * the invalid ids in the given ranges.
 *
 * As a bruteforce approach, we can search for all the invalid ids in the given
 * ranges. This means:
 * 1. Getting the index in the input vector.
 * 2. Parsing the range string.
 * 3. Checking if the range contains a repeated character.
 * 4. Summing up the invalid ids.
 * 5. Starting again
 *
 * This would cost us a total of:
 *
 * TC: O(n^2) -> we need to iterate over all the ranges and check if they
 * contain a repeated character.
 * SC: O(1) -> we only need to store the sum of
 * the invalid ids.
 *
 * An idea could be to use a helper function to check if the given number is
 * invalid. Then, in a loop we can iterate over all the ranges and check if they
 * contain a repeated character.
 *
 * To avoid allocating strings, we could use this fact:
 * A number N is invalid  if it's formed by a value H (the half) shifted and
 * added to itself: N = H * 10^d + H where d is the number of digits in H.
 *
 * N = 123123, H = 123, d = 3
 * 123 * 10^3 + 123 = 123123 -> invalid.
 *
 * We use a helper function to check if the given number is invalid.
 *
 * TC: O(log(n)) -> we need to iterate over all the digits in the number.
 * SC: O(1) -> we only need to store the digits count.
 *
 * With that, we can iterate over all the ranges and check if they contain a
 * repeated character.
 *
 * TC: O(n * log(n)) -> we need to iterate over all the ranges and check if they
 * contain a repeated character.
 * SC: O(1) -> we only need to store the sum of
 * the invalid ids.
 */

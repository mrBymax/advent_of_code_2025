#include "shared/io.hpp"
#include <iostream>
#include <vector>

void part1(const std::vector<std::string> &lines) {
  int ans = 0;
  int starting_pos = 50;

  for (const auto &line : lines) {
    // it's a circular sum!
    if (line[0] == 'L') {
      starting_pos -= std::stoi(line.substr(1));
      starting_pos %= 100;
    } else {
      starting_pos += std::stoi(line.substr(1));
      starting_pos %= 100;
    }
    if (starting_pos == 0) {
      ans++;
    }
    std::cout << "Current position: " << starting_pos << std::endl;
  }
  std::cout << ans << std::endl;
}

void part2(const std::vector<std::string> &lines) {
  int ans = 0;
  int starting_pos = 50;

  for (const auto &line : lines) {
    int steps = std::stoi(line.substr(1));

    if (line[0] == 'L') {
      for (int i = 0; i < steps; i++) {
        starting_pos--;
        if (starting_pos == 0) {
          ans++;
        } else if (starting_pos == -1) {
          starting_pos = 99;
        }
      }
    } else {
      for (int i = 0; i < steps; i++) {
        starting_pos++;
        if (starting_pos == 100) {
          starting_pos = 0;
          ans++;
        }
      }
    }
  }
  std::cout << ans << std::endl;
}

int main() {
  auto lines = aoc::io::read_lines();

  part1(lines);
  part2(lines);

  return 0;
}

/** Notes
 *
 * PART 1
 * It's a circular sum starting from 50.
 * The range is [0, 99].
 *
 * Each instruction is like "DX" where D in ['L', 'R'] and X is a natural
 * number.
 *
 * The ans is the number of times we reach the 0;
 *
 * TC: O(n) -> one pass
 * SC: O(1) -> no extra space required
 *
 * PART 2
 *
 * We now need to keep track of the times 0 is reached (both as a result and
 * while moving). Watch out for instructions like "L1000" that can cause us to
 * reach 0 multiple times (specifically 50).
 *
 * A bruteforce solution for that is just to reset the correct position after each crossing.
 *
 * TC: O(n) -> one pass (with n being in the same order as the input)
 * SC: O(1) -> no extra space required
 */

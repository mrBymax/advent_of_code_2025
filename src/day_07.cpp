#include "shared/io.hpp"
#include <iostream>
#include <numeric>
#include <vector>

void part1(const std::vector<std::string> &lines) {

  int rows = lines.size();
  int cols = lines[0].length();

  std::vector<bool> current_beam(cols, false);

  // find S
  for (int c = 0; c < cols; c++)
    if (lines[0][c] == 'S') {
      current_beam[c] = true;
      break; // there's only one starting point
    }

  long long ans = 0;
  for (int i = 0; i < rows; i++) {
    std::vector<bool> next_beam(cols, false);

    for (int c = 0; c < cols; c++) {
      if (current_beam[c]) {
        char cell = lines[i][c];

        if (cell == '.' || cell == 'S') {
          next_beam[c] = true;
        } else if (cell == '^') {
          // Hit a splitter!
          ans++;

          // Split Diagonally to the Next Row
          if (c > 0)
            next_beam[c - 1] = true;
          if (c < cols - 1)
            next_beam[c + 1] = true;
        }
      }
    }
    current_beam = next_beam;
  }
  std::cout << ans << std::endl;
}

void part2(const std::vector<std::string> &lines) {
  int rows = lines.size();
  int cols = lines[0].length();

  std::vector<long long> current_beam(cols);

  // find S
  for (int c = 0; c < cols; c++)
    if (lines[0][c] == 'S') {
      current_beam[c] = 1;
      break; // there's only one starting point
    }

  for (int i = 0; i < rows; i++) {
    std::vector<long long> next_beam(cols, 0);
    for (int c = 0; c < cols; c++) {
      if (current_beam[c] > 0) {
        if (lines[i][c] == '.' || lines[i][c] == 'S') {
          next_beam[c] += current_beam[c];
        } else if (lines[i][c] == '^') {
          if (c < cols - 1)
            next_beam[c + 1] += current_beam[c];
          if (c > 0)
            next_beam[c - 1] += current_beam[c];
        }
      }
    }
    current_beam = next_beam;
  }
  unsigned long long ans =
      std::accumulate(current_beam.begin(), current_beam.end(), 0LL);
  std::cout << ans;
}

int main() {
  auto lines = aoc::io::read_lines();

  part1(lines);
  part2(lines);

  return 0;
}

/** Notes
 * The input is a 2d grid with starting point S.
 * We can move only downwards, and we can assume the grid is always valid
 (i.e. not empty, coherent cells, etc.)
 * Each time we hit a splitter, we split our beam in 2:
 *
 * Example:
 .......S.......
 ...............
 .......^.......
 ...............
 ......^.^......
 * becomes:
 .......S.......
 .......|.......
 ......|^|......
 ...............
 ......^.^......
 * and so on.
 *
 * We can see that the split turns '^' in '|^|' and we may treat each beam
 separately?
 * Maybe not, it's a pyramid.
 *
 * How does the split occur?
 * curr = (r,c) and moves down
 * next = (r+1, c) because it can only move vertically
 * they hit a '^' and two new beams appear at (r+1,c-1) and (r+1,c+1)
 *
 * Idea:
 * Put each beam in a queue and use grid-BFS to track down the beams.
 * Every time a split occurs, enqueue two new beams.
 *
 * A better idea could be to analyse the grid row-by-row and keep track of "if
 * the beam present?"
 * This is necessary because some splitter may not be reached (that's why
 * counting the number of '^' is not enough).
 *
 * Keeping the same idea as above, but instead of pushing,
 * we set next[r+1][c+1] = true and next[r+1][c-1] = true;
 *
 * TC: O(n * m) -> we need to visit the whole grid
 * SC: O(n) -> we can store the map one row at the time
 *
 * For part 2 we're actually interested in how many beams we reach. To do this
 * we prepare each time the next row in the grid before parsing.
*/

#include "shared/io.hpp"
#include <array>
#include <iostream>
#include <string>
#include <vector>

constexpr std::array<std::pair<int, int>, 8> dirs = {
    {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};

int count_neighbours(const std::vector<std::string> &grid, int r, int c) {
  int count = 0;
  for (auto &[dr, dc] : dirs) {
    size_t nr = r + dr;
    size_t nc = c + dc;
    if (nr >= 0 && nr < grid.size() && nc >= 0 && nc < grid[0].size()) {
      if (grid[nr][nc] == '@')
        count++;
    }
  }
  return count;
}

void part1(const std::vector<std::string> &grid) {
  int rows = grid.size();
  int cols = grid[0].size();
  int ans = 0;

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      if (grid[r][c] == '@') {
        int neighbours = count_neighbours(grid, r, c);
        if (neighbours < 4)
          ans++;
      }
    }
  }

  std::cout << ans << std::endl;
}

void part2(const std::vector<std::string> &grid) {
  int rows = grid.size();
  int cols = grid[0].size();
  // copy grid
  auto working_grid = grid;
  std::vector<std::vector<int>> counts(
      rows, std::vector<int>(cols, 0)); // store neighbour counts
  std::queue<std::pair<int, int>> q;    // queue;

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      if (working_grid[r][c] != '@')
        continue;
      counts[r][c] = count_neighbours(grid, r, c);
      if (counts[r][c] < 4)
        q.push({r, c});
    }
  }

  int ans = 0;

  while (!q.empty()) {
    auto [r, c] = q.front();
    q.pop();

    // check if already processed
    if (working_grid[r][c] != '@')
      continue;

    working_grid[r][c] = 'x'; // x means removed
    ans++;

    for (const auto &[dr, dc] : dirs) {
      int nr = r + dr;
      int nc = c + dc;

      if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
        if (working_grid[nr][nc] == '@') {
          counts[nr][nc]--;
          if (counts[nr][nc] < 4)
            q.push({nr, nc});
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
 * This is a grid traversal problem. We need to count the surroundings of each
 * '@' and, if there're less than 4, '@', collect the '@' and mark it with an
 * 'x'.
 *
 * This could be a standard grid BFS problem. We should make a shadow grid to
 * count the neighbours. The main idea is to iterate on the input grid once,
 * while building the counts matrix.
 *
 * TC: O(n*m) -> we need to traverse the grid once to build the counts matrix.
 * SC: O(1) ->  we operate the grid in-place.
 *
 * For part 2, we need to keep track of the number of '@' that are removed.
 * We can do this by counting the number of '@' that are removed during the BFS.
 *
 * TC: O(n*m) -> we need to traverse the grid once to build the counts matrix.
 * SC: O(n*m) -> we make a shadow grid to count the neighbours.
 */

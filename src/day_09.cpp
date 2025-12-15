#include "shared/io.hpp"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>

struct Point {
  int x;
  int y;
};

void part1(const std::vector<std::string> &lines) {
  std::vector<Point> points;
  for (auto &line : lines) {
    std::istringstream iss(line);
    Point p;
    char c; // comma
    iss >> p.x >> c >> p.y;
    points.push_back(p);
  }

  long long ans = 0;

  for (size_t i = 0; i < points.size(); ++i) {
    for (size_t j = i + 1; j < points.size(); ++j) {
      // area = (x2 - x1 + 1) * (y2 - y1 + 1)
      long long width = std::abs(points[i].x - points[j].x) + 1;
      long long height = std::abs(points[i].y - points[j].y) + 1;
      long long area = width * height;
      ans = std::max(ans, area);
    }
  }

  std::cout << ans << std::endl;
}

void part2(const std::vector<std::string> &lines) {
  std::vector<Point> points;
  for (auto &line : lines) {
    std::istringstream iss(line);
    Point p;
    char c; // comma
    iss >> p.x >> c >> p.y;
    points.push_back(p);
  }

  int max_w = 0, max_h = 0;
  for (size_t i = 0; i < points.size(); ++i) {
    max_w = std::max(max_w, points[i].x);
    max_h = std::max(max_h, points[i].y);
  }

  std::vector<std::vector<int8_t>> grid(
      max_w + 2, std::vector<int8_t>(max_h + 2, 0)); // 0 means "invalid"

  // define the boundaries
  for (size_t i = 0; i < points.size(); ++i) {
    size_t next_idx = (i + 1) % points.size();
    Point p1 = points[i];
    Point p2 = points[next_idx];

    if (p1.x == p2.x) {
      for (int i = std::min(p1.y, p2.y); i <= std::max(p1.y, p2.y); ++i) {
        grid[p1.x][i] = 1;
      }
    } else {
      for (int i = std::min(p1.x, p2.x); i <= std::max(p1.x, p2.x); ++i) {
        grid[i][p1.y] = 1;
      }
    }
  }

  // flood fill the grid using BFS
  std::queue<Point> q;
  q.push({0, 0});

  while (!q.empty()) {
    int sz = q.size();
    while (sz--) {
      Point p = q.front();
      q.pop();
      if (grid[p.x][p.y] == 1)
        continue;
      else if (grid[p.x][p.y] == 2)
        continue;
      grid[p.x][p.y] = 2; // 2 means "visited"

      if (p.x > 0)
        q.push({p.x - 1, p.y});
      if (p.x < max_w + 1)
        q.push({p.x + 1, p.y});
      if (p.y > 0)
        q.push({p.x, p.y - 1});
      if (p.y < max_h + 1)
        q.push({p.x, p.y + 1});
    }
  }

  // grid cleanup
  for (int x = 0; x <= max_w + 1; ++x) {
    for (int y = 0; y <= max_h + 1; ++y) {
      if (grid[x][y] == 2)
        grid[x][y] = 0; // outside -> invalid
      else if (grid[x][y] == 0)
        grid[x][y] = 1; // inside (or boundary) -> valid
    }
  }

  std::vector<std::vector<long long>> prefix_sum(
      max_w + 3, std::vector<long long>(max_h + 3, 0));

  for (int i = 1; i <= max_w + 2; ++i) {
    for (int j = 1; j <= max_h + 2; ++j) {
      long long curr = grid[i - 1][j - 1];
      prefix_sum[i][j] = prefix_sum[i - 1][j] + prefix_sum[i][j - 1] -
                         prefix_sum[i - 1][j - 1] + curr;
    }
  }

  long long ans = 0;
  // iterate through all the pairs of red points and calculate the area
  // if the sum equals the area, the rectangle is valid
  for (size_t i = 0; i < points.size(); i++) {
    for (size_t j = i + 1; j < points.size(); j++) {
      long long area = (std::abs(points[i].x - points[j].x) + 1) *
                       (std::abs(points[i].y - points[j].y) + 1);

      long long x1 = std::min(points[i].x, points[j].x);
      long long x2 = std::max(points[i].x, points[j].x);
      long long y1 = std::min(points[i].y, points[j].y);
      long long y2 = std::max(points[i].y, points[j].y);

      long long sum = prefix_sum[x2 + 1][y2 + 1] - prefix_sum[x1][y2 + 1] -
                      prefix_sum[x2 + 1][y1] + prefix_sum[x1][y1];

      if (sum == area)
        ans = std::max(ans, area);
    }
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
 * The goal is to find the largest possible rectangle that can be formed using
 * the given points. The points are given as pairs of integers representing
 * their x and y coordinates.
 *
 * A naive approach would be to iterate over all possible pairs of points and
 * check if they form a rectangle. This would result in a time complexity of
 * O(n^2), where n is the number of points.
 *
 * Part two is completely different. In part two, we need to find a rectangle
 * that fits the problem description.
 *
 * A 2D prefix sum can be used to efficiently calculate the sum of elements in
 * a rectangle. We can precompute the prefix sum for each point and then use it
 * to calculate the sum of elements in any rectangle in constant time.
 *
 * We have a valid rectangle if the sum of elements in the rectangle is equal
 * to the area of the rectangle.
 *
 * TC: O(n^2) -> we need to traverse the grid
 * SC: O(n^2) -> we need to store the prefix sum for each point
 */

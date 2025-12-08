#include "shared/io.hpp"
#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::vector<std::string>>
transpose_tokens(const std::vector<std::vector<std::string>> &matrix) {
  if (matrix.empty())
    return {};

  size_t max_width = 0;
  for (const auto &row : matrix) {
    max_width = std::max(max_width, row.size());
  }

  std::vector<std::vector<std::string>> transposed(
      max_width, std::vector<std::string>(matrix.size()));
  for (size_t i = 0; i < matrix.size(); ++i) {
    for (size_t j = 0; j < matrix[i].size(); ++j) {
      transposed[j][i] = matrix[i][j];
    }
  }
  return transposed;
}

void part1(const std::vector<std::string> &lines) {
  std::vector<std::vector<std::string>> input_matrix;

  for (const auto &line : lines) {
    std::vector<std::string> row;
    std::istringstream iss(line);
    std::string token;
    while (iss >> token) {
      row.push_back(token);
    }
    input_matrix.push_back(row);
  }

  auto matrix = transpose_tokens(input_matrix);

  long long ans = 0;
  for (auto &row : matrix) {
    if (row.size() < 2)
      continue;

    long long row_res = 0;
    std::string op = row.back();

    if (op == "+") {
      for (size_t i = 0; i < row.size() - 1; ++i) {
        if (row[i].empty())
          continue; // Skip padding from transpose
        row_res += std::stoll(row[i]);
      }
    } else if (op == "*") {
      row_res = 1; // Initialize multiplication identity
      for (size_t i = 0; i < row.size() - 1; ++i) {
        if (row[i].empty())
          continue; // Skip padding
        row_res *= std::stoll(row[i]);
      }
    }

    ans += row_res;
  }

  std::cout << ans << std::endl;
}

long long solve_block(const std::vector<std::string> &cols) {
  if (cols.empty())
    return 0;

  char op = 0;
  std::vector<long long> numbers;

  for (const auto &col : cols) {
    char bottom_char = col.back();
    if (bottom_char == '+' || bottom_char == '*') {
      op = bottom_char;
    }

    std::string num_str;
    for (char c : col) {
      if (std::isdigit(c)) {
        num_str += c;
      }
    }

    if (!num_str.empty()) {
      numbers.push_back(std::stoll(num_str));
    }
  }

  if (numbers.empty())
    return 0;

  long long res = numbers[0];
  for (size_t i = 1; i < numbers.size(); ++i) {
    if (op == '+')
      res += numbers[i];
    else if (op == '*')
      res *= numbers[i];
  }

  return res;
}

void part2(const std::vector<std::string> &lines) {
  if (lines.empty())
    return;
  size_t max_width = 0;
  for (const auto &line : lines)
    max_width = std::max(max_width, line.size());

  std::vector<std::string> grid = lines;
  for (auto &row : grid) {
    row.resize(max_width, ' ');
  }

  long long ans = 0;
  size_t height = grid.size();

  // Buffer to hold columns for the current problem
  std::vector<std::string> cols;
  for (int x = (int)max_width - 1; x >= -1; --x) {
    bool is_separator = false;
    std::string col_chars;

    if (x == -1) {
      is_separator = true;
    } else {
      bool has_content = false;
      for (size_t y = 0; y < height; ++y) {
        char c = grid[y][x];
        col_chars += c;
        if (c != ' ')
          has_content = true;
      }
      is_separator = !has_content;
    }

    if (is_separator) {
      if (!cols.empty()) {
        ans += solve_block(cols);
        cols.clear();
      }
    } else {
      // Add valid column to buffer
      cols.push_back(col_chars);
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
 * This is like "basic calculator" from leetcode.
 * The input is formed like this:
 * The numbers are arranged vertically, at the bottom there's the operator.
 * Each number is separated (horizontally) by a column of spaces.
 * After solving each problem, the results needs to be summed up.
 *
 * Idea: we can transpose the matrix in order to have the numbers and symbols
 * in the same row.
 *
 * TC: O(n*m) -> the bottleneck is the transposition. It's enough to scan the
 * input once. SC: O(n*m) -> we need to store the transposed matrix.
 *
 * Part two reads the problem differently. Each number is given in its own
 * column, with the most significant digit at the top and the least
 * significant digit at the bottom, and the problem is read column-wise from
 * right to left.
 *
 * Example:
 * 123 328  51  64
 *  45  64 387  23
 *   6  98 215 314
 *   *   +   *   +
 *
 * We have:
 * - rightmost 4 + 431 + 623 = 1058
 * - second rightmost 175 * 581 * 32 = 325600
 * ...
 *
 * Here we cannot transpose anymore because we need to read the numbers
 * column-wise.
 */

#include "io.hpp"
#include <iterator>

namespace aoc::io {

std::string read_all(std::istream &in) {
  // Optimization: Disable sync with stdio for speed
  std::ios::sync_with_stdio(false);
  in.tie(nullptr);

  // Use an iterator to read the stream from start to finish
  return std::string(std::istreambuf_iterator<char>(in), {});
}

std::vector<std::string> read_lines(std::istream &in) {
  std::vector<std::string> lines;
  std::string line;

  while (std::getline(in, line)) {
    if (!line.empty()) { // Optional: skip empty lines depending on problem
      lines.push_back(line);
    }
  }
  return lines;
}

std::vector<std::string> read_grid(std::istream &in) {
  // In AoC, a grid is just a vector of strings where each string is a row.
  // Usually, padding is required, but we'll stick to raw reading here.
  return read_lines(in);
}
} // namespace aoc::io

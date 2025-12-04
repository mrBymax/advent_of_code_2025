#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace aoc::io {

/**
 * @brief Reads the entire standard input into a single string.
 * Useful when the input is a single block or complex parsing is needed.
 */
std::string read_all(std::istream &in = std::cin);

/**
 * @brief Reads standard input line by line.
 * Useful for most AoC problems where inputs are line-delimited.
 */
std::vector<std::string> read_lines(std::istream &in = std::cin);

/**
 * @brief Reads standard input as a grid of characters.
 * Essential for maze/map problems.
 */
std::vector<std::string> read_grid(std::istream &in = std::cin);

} // namespace aoc::io

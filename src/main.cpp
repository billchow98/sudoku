// © 2023 Bill Chow. All rights reserved.
// Unauthorized use, modification, or distribution of this code is strictly
// prohibited.

#include <absl/strings/str_format.h>

#include <cstdlib>
#include <string>

#include "sudoku.h"

const std::string usage_str = R"(Usage: sudoku <puzzle_string>

<puzzle_string>:
  A string representing the Sudoku puzzle to be solved. The string must be exactly 81 characters long.
  Each character represents a cell in the Sudoku grid:
    - Dots ('.') represent empty cells.
    - Digits ('1'-'9') represent filled cells.
  The first 9 characters represent the top row of the grid, the next 9 characters represent the second row, and so on.

Example:
  sudoku ..3.2.6..9..3.5..1..18.64....81.29..7.......8..67.82....26.95..8..2.3..9..5.1.3..

Description:
  The provided example represents the following Sudoku grid:

  . . 3 | . 2 . | 6 . .
  9 . . | 3 . 5 | . . 1
  . . 1 | 8 . 6 | 4 . .
  ------|-------|-------
  . . 8 | 1 . 2 | 9 . .
  7 . . | . . . | . . 8
  . . 6 | 7 . 8 | 2 . .
  ------|-------|-------
  . . 2 | 6 . 9 | 5 . .
  8 . . | 2 . 3 | . . 9
  . . 5 | . 1 . | 3 . .

Notes:
  - Ensure that the puzzle_string is exactly 81 characters.
  - Only use digits ('1'-'9') and dots ('.').
)";

int main(const int argc, char** argv) {
  if (argc != 2) {
    absl::PrintF("%v\n", usage_str);
    exit(EXIT_FAILURE);
  }
  const sudoku::Result res = sudoku::solve(argv[1]);
  if (res.is_error()) {
    absl::FPrintF(stderr, "%v\n", res);
    exit(EXIT_FAILURE);
  }
  absl::PrintF("%v\n", res);
  return 0;
}

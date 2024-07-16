// © 2023 Bill Chow. All rights reserved.
// Unauthorized use, modification, or distribution of this code is strictly
// prohibited.

#ifndef SUDOKU_SUDOKU_H
#define SUDOKU_SUDOKU_H

#include <string>
#include <string_view>

#include "util.h"

namespace sudoku {

typedef sudoku_util::Result<std::string> Result;

Result solve(std::string_view input);

}  // namespace sudoku

#endif  // SUDOKU_SUDOKU_H

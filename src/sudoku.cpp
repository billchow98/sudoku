// © 2023 Bill Chow. All rights reserved.
// Unauthorized use, modification, or distribution of this code is strictly
// prohibited.

#include "sudoku.h"

#include <absl/strings/str_format.h>

#include <algorithm>
#include <bitset>

namespace sudoku {

namespace {

class SudokuSolver {
 public:
  SudokuSolver() {
    std::fill_n(grid, 81, -1);
    std::fill_n(candidates, 81, 0b111'111'111);
  }

  Result solve(const std::string_view input) {
    if (Result res = is_valid_input(input); res.is_error()) {
      return res;
    }
    if (Result res = initialize_grid(input); res.is_error()) {
      return res;
    }
    if (!dfs()) {
      return Result::from_error("no solution");
    }
    verify_grid();
    return Result::from_value(grid_as_string());
  }

 private:
  template <class... StrFormatArgs>
  Result make_invalid_input_error(StrFormatArgs &&...args) {
    const std::string msg =
        absl::StrFormat(std::forward<StrFormatArgs>(args)...);
    return Result::from_error(absl::StrFormat("invalid input (%v)", msg));
  }

  Result is_valid_input(const std::string_view input) {
    if (input.length() != 81) {
      return make_invalid_input_error("len = %v", input.length());
    }
    for (int i = 0; i < 81; i++) {
      if (input[i] == '.' || '1' <= input[i] && input[i] <= '9') {
        continue;
      }
      return make_invalid_input_error("'%c' at pos %v", input[i], i);
    }
    return EMPTY_RESULT;
  }

  // Initialize the grid and candidates bitset at the same time
  Result initialize_grid(const std::string_view input) {
    for (int i = 0; i < 81; i++) {
      if (const char ch = input[i]; ch == '.') {
        grid[i] = -1;
      } else {
        assert('1' <= ch && ch <= '9');
        grid[i] = ch - '1';
        // Candidate was already eliminated. So there is a duplicate digit
        // in a group. The puzzle is unsolvable.
        if (!candidates[i][grid[i]]) {
          return Result::from_error("no solution");
        }
        update_candidates(i);
      }
    }
    return EMPTY_RESULT;
  }

  void update_group(const int pos, const int start_pos, const int *const inc) {
    assert(grid[pos] != -1);
    int i = start_pos;
    for (int j = 0; j < 9; j++) {
      i += inc[j];
      candidates[i][grid[pos]] = false;
    }
  }

  void update_row(const int pos) {
    return update_group(pos, pos / 9 * 9, row_inc);
  }

  void update_col(const int pos) { return update_group(pos, pos % 9, col_inc); }

  void update_box(const int pos) {
    const int box_start_row = pos / 9 / 3 * 3;
    const int box_start_col = pos % 9 / 3 * 3;
    return update_group(pos, box_start_row * 9 + box_start_col, box_inc);
  }

  void update_candidates(const int pos) {
    update_row(pos);
    update_col(pos);
    update_box(pos);
  }

  // Runs DFS and returns error if no solution was found
  bool dfs() {
    // Make a deep copy of the data as a backup
    const SudokuSolver backup_solver = *this;

    int filled_cnt = 0;
    for (int i = 0; i < 81; i++) {
      filled_cnt += grid[i] != -1;
      // Return if unsolvable
      if (grid[i] == -1 && candidates[i].none()) {
        return false;
      }
    }

    // Return if fully solved
    if (filled_cnt == 81) {
      return true;
    }

    // Look for best cell to try next (least number of candidates left)
    int best_cnt = 10;
    int best_pos = -1;
    for (int i = 0; i < 81; i++) {
      if (grid[i] != -1) {
        continue;
      }
      if (candidates[i].count() < best_cnt) {
        best_cnt = static_cast<int>(candidates[i].count());
        best_pos = i;
      }
    }

    // Fill in the chosen cell
    assert(grid[best_pos] == -1);
    for (int i = 0; i < 9; i++) {
      if (candidates[best_pos][i]) {
        grid[best_pos] = i;
        update_candidates(best_pos);
        if (dfs()) {
          // Do not restore original data as this is the solution
          return true;
        }
        // Restore new data with original for next iteration
        *this = backup_solver;
      }
    }

    // No solutions in current search space
    return false;
  }

  // Verify the grid is completely filled
  // Note: No guarantee the grid is valid
  void verify_grid() const {
    for (const int i : grid) {
      assert(i != -1);
    }
  }

  [[nodiscard]] std::string grid_as_string() const {
    std::string sln_str;
    for (int i = 0; i < 81; i++) {
      if (i != 0 && i % 9 == 0) {
        sln_str += '\n';
      }
      sln_str += static_cast<char>(grid[i] + '1');
    }
    return sln_str;
  }

  static constexpr int row_inc[9] = {0, 1, 1, 1, 1, 1, 1, 1, 1};
  static constexpr int col_inc[9] = {0, 9, 9, 9, 9, 9, 9, 9, 9};
  static constexpr int box_inc[9] = {0, 1, 1, 7, 1, 1, 7, 1, 1};

  inline const static Result EMPTY_RESULT = Result::from_value("");

  int grid[81]{};
  std::bitset<9> candidates[81];
};

}  // namespace

Result solve(const std::string_view input) {
  SudokuSolver solver;
  return solver.solve(input);
}

}  // namespace sudoku

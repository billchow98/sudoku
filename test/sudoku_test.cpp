// © 2023 Bill Chow. All rights reserved.
// Unauthorized use, modification, or distribution of this code is strictly
// prohibited.

#include "../src/sudoku.h"

#include <absl/strings/str_cat.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <fstream>
#include <string>
#include <string_view>
#include <tuple>

TEST(SudokuTest, InputValidation) {
  const std::tuple<std::string, bool, std::string> tcs[] = {
      {"", true, "invalid input (len = 0)"},
      {std::string(80, '.'), true, "invalid input (len = 80)"},
      {std::string(80, '.') + '#', true, "invalid input ('#' at pos 80)"},
      {std::string(79, '.') + "11", true, "no solution"},
      {std::string(81, '.'), false, ""},
      {std::string(82, '.'), true, "invalid input (len = 82)"},
  };
  for (const auto &[input, has_error, output] : tcs) {
    sudoku::Result res = sudoku::solve(input);
    ASSERT_EQ(has_error, res.is_error());
    if (has_error) {
      ASSERT_EQ(output, res.get_error_string());
    }
  }
}

void test_with_files(const std::string &input_filename,
                     const std::string &output_filename) {
  auto in = std::ifstream(input_filename);
  ASSERT_TRUE(in.is_open());
  auto out = std::ifstream(output_filename);
  ASSERT_TRUE(out.is_open());
  std::string input;
  std::string output;
  while (std::getline(in, input)) {
    ASSERT_TRUE(input.length() == 81);
    std::getline(out, output);
    ASSERT_TRUE(output.length() == 81);
    sudoku::Result res = sudoku::solve(input);
    ASSERT_FALSE(res.is_error());
    std::string v = res.get_value();
    v.erase(std::remove(v.begin(), v.end(), '\n'), v.end());
    EXPECT_EQ(output, v);
  }
}

TEST(SudokuTest, SolverCheck) {
  using namespace absl;
  auto r = [](const std::string_view name) {
    return StrCat(TESTCASES_PATH, "/", name);
  };
  auto t = [r](const std::string_view base_name) {
    test_with_files(r(StrCat(base_name, ".in.txt")),
                    r(StrCat(base_name, ".out.txt")));
  };
  t("easy50");
  t("hard95");
  t("hardest11");
}

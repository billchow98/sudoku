// © 2023 Bill Chow. All rights reserved.
// Unauthorized use, modification, or distribution of this code is strictly
// prohibited.

#ifndef SUDOKU_UTIL_H
#define SUDOKU_UTIL_H

#include <absl/strings/str_format.h>

#include <cassert>
#include <string>
#include <string_view>
#include <variant>

namespace sudoku_util {

class ErrorString : public std::string {
  using std::string::basic_string;
};

template <class T>
class Result : std::variant<T, ErrorString> {
  using std::variant<T, ErrorString>::variant;

 public:
  static Result from_value(const T &v) { return Result{v}; }

  static Result from_error(const std::string_view err_msg) {
    return Result{ErrorString(err_msg)};
  }

  [[nodiscard]] bool is_ok() const { return std::holds_alternative<T>(*this); }

  [[nodiscard]] bool is_error() const {
    return std::holds_alternative<ErrorString>(*this);
  }

  const T &get_value() const {
    assert(is_ok());
    return std::get<T>(*this);
  }

  [[nodiscard]] const std::string &get_error_string() const {
    assert(is_error());
    return std::get<ErrorString>(*this);
  }

  template <typename Sink>
  friend void AbslStringify(Sink &sink, const Result &res) {
    if (res.is_ok()) {
      absl::Format(&sink, "%v", res.get_value());
    } else {
      absl::Format(&sink, "%s", res.get_error_string());
    }
  }
};

}  // namespace sudoku_util

#endif  // SUDOKU_UTIL_H

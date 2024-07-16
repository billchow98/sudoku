add_executable(sudoku_test sudoku_test.cpp ../src/sudoku.cpp)

target_include_directories(sudoku_test PRIVATE ../third_party/abseil-cpp ../third_party/googletest/googletest/include)

target_link_libraries(sudoku_test absl::strings absl::str_format GTest::gtest GTest::gtest_main)

target_compile_definitions(sudoku_test PRIVATE "TESTCASES_PATH=\"$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/testcases>\"")

include(GoogleTest)

gtest_discover_tests(sudoku_test)

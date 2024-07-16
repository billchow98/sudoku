cmake_minimum_required(VERSION 3.20)

project(sudoku)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(third_party/abseil-cpp)
add_subdirectory(third_party/googletest)

add_executable(sudoku src/main.cpp src/sudoku.cpp)

target_include_directories(sudoku PRIVATE third_party/abseil-cpp)

target_link_libraries(sudoku absl::str_format)

enable_testing()

add_subdirectory(test)

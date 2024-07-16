#!/bin/bash

# © 2023 Bill Chow. All rights reserved.
# Unauthorized use, modification, or distribution of this code is strictly prohibited.

set -euxo pipefail

cmake -S . -B ./build -G Ninja && cd build && ninja sudoku_test
test/sudoku_test

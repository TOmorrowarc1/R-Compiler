#!/bin/bash

set -e

echo "--- Configuring CMake ---"
cmake -B build

echo "--- Building project ---"
cmake --build build --config Release

echo "--- Running tests ---"
cd build
ctest -C Release --verbose
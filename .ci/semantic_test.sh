#!/bin/bash

set -e

echo "--- Configuring CMake ---"
cmake -B build

echo "--- Building project ---"
cmake --build build --config Release

echo "--- Running semantic tests ---"
./build/tests/run_tests --gtest_filter=SemanticTest.*
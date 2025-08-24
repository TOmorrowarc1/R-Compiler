#pragma once
#include "gtest/gtest.h"

extern const std::string test_cases_dir;

auto readFileToString(const std::string &filepath) -> std::string;
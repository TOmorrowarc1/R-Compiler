#pragma once
#include "gtest/gtest.h"
#include "ASTAnnotator.hpp"
#include "ASTRootNode.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "exception.hpp"

extern const std::string test_cases_dir;

auto readFileToString(const std::string &filepath) -> std::string;
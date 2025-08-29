#pragma once
#include "ASTAnnotator.hpp"
#include "ASTRootNode.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Scope.hpp"
#include "exception.hpp"
#include "gtest/gtest.h"

extern const std::string test_cases_dir;

auto readFileToString(const std::string &filepath) -> std::string;
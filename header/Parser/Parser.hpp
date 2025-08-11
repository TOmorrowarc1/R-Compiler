#pragma once
#include "cast.hpp"
#include "token.hpp"
#include <memory>
#include <vector>

class ASTRootNode;

auto parse(const std::vector<Token> &tokens) -> std::unique_ptr<ASTRootNode>;
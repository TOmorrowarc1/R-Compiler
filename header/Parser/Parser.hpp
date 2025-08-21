#pragma once
#include "Token.hpp"
#include <memory>
#include <vector>

class ASTRootNode;

auto parse(const std::vector<Token> &tokens) -> std::unique_ptr<ASTRootNode>;
#pragma once
#include "token.hpp"
#include <memory>
#include <vector>

class ASTNode;

auto parse(const std::vector<Token> &tokens) -> std::unique_ptr<ASTNode>;
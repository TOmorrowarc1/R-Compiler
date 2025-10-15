#pragma once
#include "Data/Token/Token.hpp"
#include <memory>
#include <vector>

/*
ASTNodes are introduced for both Parser, Semantic Checker and IR builder, in
other words, AST coorperating the Scope Tree mentioned later is the basis of all
remaining stages of the frontend. The job taken by the parser is construct the
AST from the token stream by recursive descent and fetching key information.
*/

class ASTRootNode;

auto parse(const std::vector<Token> &tokens) -> std::unique_ptr<ASTRootNode>;
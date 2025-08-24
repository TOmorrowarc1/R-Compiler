#include "ASTAnnotator.hpp"
#include "ASTRootNode.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include <iostream>
#include <sstream>

int main() {
  std::stringstream buffer;
  buffer << std::cin.rdbuf();
  std::string text = buffer.str();

  auto lex_result = lex(text);
  auto AST_root = parse(lex_result);

  Scope init_scope;
  auto root = AST_root.get();
  ASTAnnotate(root, &init_scope);
  return 0;
}
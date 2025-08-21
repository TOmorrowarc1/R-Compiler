#include "ASTAnnotator.hpp"
#include "ASTRootNode.hpp"
#include "Parser.hpp"
#include "lexer.hpp"
#include <chrono>
#include <iostream>
#include <sstream>

int main() {
  auto start = std::chrono::steady_clock::now();
  std::stringstream buffer;
  buffer << std::cin.rdbuf();
  std::string text = buffer.str();

  auto lex_result = lex(text);
  auto AST_root = parse(lex_result);

  Scope init_scope;
  auto root = AST_root.get();
  ASTAnnotate(root, &init_scope);

  auto end = std::chrono::steady_clock::now();
  auto duration = end - start;
  auto ms_duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration);
  std::cout << "run time:" << ms_duration.count();
  return 0;
}
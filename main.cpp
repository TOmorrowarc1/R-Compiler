#include "lexer.hpp"
#include <iostream>

int main() {
  std::string text;
  std::cin >> text;
  auto lexer_result = lex(text);
  for (auto iter = lexer_result.begin(); iter != lexer_result.end(); ++iter) {
    std::cout << iter->content << '\n';
  }
  return 0;
}
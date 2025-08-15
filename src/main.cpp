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
  for (auto iter = lex_result.begin(); iter != lex_result.end(); ++iter) {
    std::cout << iter->content << '\n';
  }

  auto end = std::chrono::steady_clock::now();
  auto duration = end - start;
  auto ms_duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(duration);
  std::cout << "run time:" << ms_duration.count();
  return 0;
}
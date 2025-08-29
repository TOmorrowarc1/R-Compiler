#include "ASTAnnotator.hpp"
#include "ASTRootNode.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Scope.hpp"
#include "exception.hpp"
#include <iostream>
#include <sstream>


int main() {
  std::stringstream buffer;
  buffer << std::cin.rdbuf();
  std::string text = buffer.str();

  try {
    auto lex_result = lex(text);
    for (auto &token : lex_result) {
      std::cout << token.content << "\n";
    }
    auto AST_root = parse(lex_result);

    Scope init_scope;
    auto root = AST_root.get();
    ASTAnnotate(root, &init_scope);

  } catch (CompilerException &e) {
    std::cout << e.getExceptionMessage() << '\n';
    return 1;
  } catch (std::exception &e) {
    std::cout << e.what() << '\n';
    return 1;
  }

  return 0;
}
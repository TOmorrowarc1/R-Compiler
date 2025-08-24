#include "ASTAnnotator.hpp"
#include "ASTRootNode.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "exception.hpp"
#include "utils.hpp"

TEST(AllTest, VaildAll) {
  std::string input_path = test_cases_dir + "/lexer/all_1.in";
  std::string text = readFileToString(input_path);
  try {
    auto lex_result = lex(text);
    auto parse_result = parse(lex_result);

    Scope init_scope;
    auto root = parse_result.get();
    ASTAnnotate(root, &init_scope);
  } catch (const CompilerException &e) {
    std::cout << e.getExceptionMessage() << '\n';
  }
}
#include "utils.hpp"

TEST(ParserTest, VaildParse) {
  std::string input_path = test_cases_dir + "/parser/scope_1.in";
  std::string text = readFileToString(input_path);
  try {
    auto lex_result = lex(text);
    auto parse_result = parse(lex_result);
  } catch (const CompilerException &e) {
    std::cout << e.getExceptionMessage() << '\n';
  }
}
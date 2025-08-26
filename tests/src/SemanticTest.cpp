#include "utils.hpp"

TEST(SemanticTest, Scope) {
  std::string input_path = test_cases_dir + "/Annotator/scope_1.in";
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

TEST(SemanticTest, Return) {
  std::string input_path = test_cases_dir + "/Semantic/control_1.in";
  std::string text = readFileToString(input_path);
  try {
    auto lex_result = lex(text);
    auto parse_result = parse(lex_result);

    Scope init_scope;
    auto root = parse_result.get();
    ASTAnnotate(root, &init_scope);

  } catch (const CompilerException &e) {
    std::cout << e.getExceptionMessage() << '\n';
  } catch (const std::runtime_error &e) {
    std::cout << e.what() << '\n';
  }
}

TEST(SemanticTest, Break) {
  std::string input_path = test_cases_dir + "/Semantic/control_2.in";
  std::string text = readFileToString(input_path);
  try {
    auto lex_result = lex(text);
    auto parse_result = parse(lex_result);

    Scope init_scope;
    auto root = parse_result.get();
    ASTAnnotate(root, &init_scope);

  } catch (const CompilerException &e) {
    std::cout << e.getExceptionMessage() << '\n';
  } catch (const std::runtime_error &e) {
    std::cout << e.what() << '\n';
  }
}
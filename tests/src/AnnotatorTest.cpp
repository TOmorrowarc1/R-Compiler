#include "utils.hpp"

TEST(AnnotatorTest, Literal1) {
  std::string input_path = test_cases_dir + "/Annotator/literal1.in";
  std::string text = readFileToString(input_path);
  bool flag = true;
  try {
    auto lex_result = lex(text);
    auto parse_result = parse(lex_result);
    Scope init_scope;
    auto root = parse_result.get();
    ASTAnnotate(root, &init_scope);
  } catch (const CompilerException &e) {
    std::cout << e.getExceptionMessage() << '\n';
    flag = false;
  }
  EXPECT_TRUE(flag);
}

TEST(AnnotatorTest, Impl1) {
  std::string input_path = test_cases_dir + "/Annotator/impl1.in";
  std::string text = readFileToString(input_path);
  bool flag = true;
  try {
    auto lex_result = lex(text);
    auto parse_result = parse(lex_result);
    Scope init_scope;
    auto root = parse_result.get();
    ASTAnnotate(root, &init_scope);
  } catch (const CompilerException &e) {
    std::cout << e.getExceptionMessage() << '\n';
    flag = false;
  }
  EXPECT_TRUE(flag);
}

TEST(AnnotatorTest, ConstTypeSolveTest1) {
  std::string input_path = test_cases_dir + "/Annotator/solve1-1.in";
  std::string text = readFileToString(input_path);
  bool flag = true;
  try {
    auto lex_result = lex(text);
    auto parse_result = parse(lex_result);
    Scope init_scope;
    auto root = parse_result.get();
    ASTAnnotate(root, &init_scope);
  } catch (const CompilerException &e) {
    std::cout << e.getExceptionMessage() << '\n';
    flag = false;
  }
  EXPECT_TRUE(!flag);

  std::string input_path = test_cases_dir + "/Annotator/solve1-2.in";
  std::string text = readFileToString(input_path);
  bool flag = true;
  try {
    auto lex_result = lex(text);
    auto parse_result = parse(lex_result);
    Scope init_scope;
    auto root = parse_result.get();
    ASTAnnotate(root, &init_scope);
  } catch (const CompilerException &e) {
    std::cout << e.getExceptionMessage() << '\n';
    flag = false;
  }
  EXPECT_TRUE(!flag);

  std::string input_path = test_cases_dir + "/Annotator/solve1-3.in";
  std::string text = readFileToString(input_path);
  bool flag = true;
  try {
    auto lex_result = lex(text);
    auto parse_result = parse(lex_result);
    Scope init_scope;
    auto root = parse_result.get();
    ASTAnnotate(root, &init_scope);
  } catch (const CompilerException &e) {
    std::cout << e.getExceptionMessage() << '\n';
    flag = false;
  }
  EXPECT_TRUE(flag);
}

TEST(AnnotatorTest, ConstTypeSolveTest2) {
  std::string input_path = test_cases_dir + "/Annotator/solve2.in";
  std::string text = readFileToString(input_path);
  bool flag = true;
  try {
    auto lex_result = lex(text);
    auto parse_result = parse(lex_result);
    Scope init_scope;
    auto root = parse_result.get();
    ASTAnnotate(root, &init_scope);
  } catch (const CompilerException &e) {
    std::cout << e.getExceptionMessage() << '\n';
    flag = false;
  }
  EXPECT_TRUE(flag);
}
TEST(AnnotatorTest, ConstTypeSolveTest3) {
  std::string input_path = test_cases_dir + "/Annotator/solve3.in";
  std::string text = readFileToString(input_path);
  bool flag = true;
  try {
    auto lex_result = lex(text);
    auto parse_result = parse(lex_result);
    Scope init_scope;
    auto root = parse_result.get();
    ASTAnnotate(root, &init_scope);
  } catch (const CompilerException &e) {
    std::cout << e.getExceptionMessage() << '\n';
    flag = false;
  }
  EXPECT_TRUE(flag);
}

TEST(AnnotatorTest, ConstTypeSolveTest4) {
  std::string input_path = test_cases_dir + "/Annotator/solve4.in";
  std::string text = readFileToString(input_path);
  bool flag = true;
  try {
    auto lex_result = lex(text);
    auto parse_result = parse(lex_result);
    Scope init_scope;
    auto root = parse_result.get();
    ASTAnnotate(root, &init_scope);
  } catch (const CompilerException &e) {
    std::cout << e.getExceptionMessage() << '\n';
    flag = false;
  }
  EXPECT_TRUE(flag);
}

TEST(AnnotatorTest, ConstTypeSolveTest4) {
  std::string input_path = test_cases_dir + "/Annotator/solve5.in";
  std::string text = readFileToString(input_path);
  bool flag = true;
  try {
    auto lex_result = lex(text);
    auto parse_result = parse(lex_result);
    Scope init_scope;
    auto root = parse_result.get();
    ASTAnnotate(root, &init_scope);
  } catch (const CompilerException &e) {
    std::cout << e.getExceptionMessage() << '\n';
    flag = false;
  }
  EXPECT_TRUE(flag);
}

TEST(AnnotatorTest, Scope1) {
  std::string input_path = test_cases_dir + "/Annotator/scope_1.in";
  std::string text = readFileToString(input_path);
  bool flag = true;
  try {
    auto lex_result = lex(text);
    auto parse_result = parse(lex_result);

    Scope init_scope;
    auto root = parse_result.get();
    ASTAnnotate(root, &init_scope);
  } catch (const CompilerException &e) {
    std::cout << e.getExceptionMessage() << '\n';
    flag = false;
  }
  EXPECT_TRUE(flag);
}

TEST(AnnotatorTest, Return) {
  std::string input_path = test_cases_dir + "/Annotator/control_1.in";
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

TEST(AnnotatorTest, Break) {
  std::string input_path = test_cases_dir + "/Annotator/control_2.in";
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
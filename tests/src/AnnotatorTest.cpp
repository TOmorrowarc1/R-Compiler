#include "utils.hpp"

auto endToEndTest(std::string input_path) -> bool {
  std::string real_path = test_cases_dir + "/Annotator/" + input_path;
  std::string text = readFileToString(real_path);
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
  } catch (const std::exception &e) {
    flag = false;
  }
  return flag;
}

TEST(AnnotatorTest, Array1) { EXPECT_TRUE(endToEndTest("array1.in")); }

TEST(AnnotatorTest, Array2) {
  EXPECT_TRUE(endToEndTest("array2-1.in"));
  EXPECT_FALSE(endToEndTest("array2-2.in"));
}

TEST(AnnotatorTest, Condition1) { EXPECT_TRUE(endToEndTest("condition1.in")); }

TEST(AnnotatorTest, ControlFlow1) {
  EXPECT_TRUE(endToEndTest("controlflow1.in"));
}

TEST(AnnotatorTest, ControlFlow2) {
  EXPECT_FALSE(endToEndTest("controlflow2.in"));
}

TEST(AnnotatorTest, ControlFlow3) {
  EXPECT_FALSE(endToEndTest("controlflow3.in"));
}

TEST(AnnotatorTest, ControlFlow4) {
  EXPECT_TRUE(endToEndTest("controlflow4-1.in"));
  EXPECT_FALSE(endToEndTest("controlflow4-2.in"));
}

TEST(AnnotatorTest, Impl1) { EXPECT_TRUE(endToEndTest("impl1.in")); }

TEST(AnnotatorTest, Impl2) { EXPECT_TRUE(endToEndTest("impl2.in")); }

TEST(AnnotatorTest, Impl3) {
  EXPECT_TRUE(endToEndTest("impl3-1.in"));
  EXPECT_FALSE(endToEndTest("impl3-2.in"));
}

TEST(AnnotatorTest, Literal1) { EXPECT_TRUE(endToEndTest("literal1.in")); }

TEST(AnnotatorTest, Mut1) { EXPECT_FALSE(endToEndTest("mut1.in")); }

TEST(AnnotatorTest, Mut2) { EXPECT_TRUE(endToEndTest("mut2.in")); }

TEST(AnnotatorTest, Mut3) { EXPECT_TRUE(endToEndTest("mut3.in")); }

TEST(AnnotatorTest, Pattern1) { EXPECT_TRUE(endToEndTest("pattern1.in")); }

TEST(AnnotatorTest, Pattern2) { EXPECT_FALSE(endToEndTest("pattern2.in")); }

TEST(AnnotatorTest, Scope1) {
  EXPECT_TRUE(endToEndTest("scope1-1.in"));
  EXPECT_TRUE(endToEndTest("scope2-2.in"));
}

TEST(AnnotatorTest, Shadow1) { EXPECT_TRUE(endToEndTest("shadow1.in")); }

TEST(AnnotatorTest, Shadow2) { EXPECT_FALSE(endToEndTest("shadow2.in")); }

TEST(AnnotatorTest, Shadow3) { EXPECT_TRUE(endToEndTest("shadow3.in")); }

TEST(AnnotatorTest, Shadow4) { EXPECT_FALSE(endToEndTest("shadow4.in")); }

TEST(AnnotatorTest, Shadow5) { EXPECT_FALSE(endToEndTest("shadow5.in")); }

TEST(AnnotatorTest, Shadow6) { EXPECT_FALSE(endToEndTest("shadow6.in")); }

TEST(AnnotatorTest, Shadow7) { EXPECT_FALSE(endToEndTest("shadow7.in")); }

TEST(AnnotatorTest, Solve1) {
  EXPECT_FALSE(endToEndTest("solve1-1.in"));
  EXPECT_FALSE(endToEndTest("solve1-2.in"));
  EXPECT_TRUE(endToEndTest("solve1-3.in"));
}

TEST(AnnotatorTest, Solve2) { EXPECT_FALSE(endToEndTest("solve2.in")); }

TEST(AnnotatorTest, Solve3) { EXPECT_TRUE(endToEndTest("solve3.in")); }

TEST(AnnotatorTest, Solve4) { EXPECT_TRUE(endToEndTest("solve4.in")); }

TEST(AnnotatorTest, Solve5) { EXPECT_TRUE(endToEndTest("solve5.in")); }

TEST(AnnotatorTest, Type1) {
  EXPECT_TRUE(endToEndTest("type1-1.in"));
  EXPECT_FALSE(endToEndTest("type1-2.in"));
}

TEST(AnnotatorTest, Type2) { EXPECT_TRUE(endToEndTest("type2.in")); }

TEST(AnnotatorTest, Type3) { EXPECT_TRUE(endToEndTest("type3.in")); }

TEST(AnnotatorTest, Type4) {
  EXPECT_TRUE(endToEndTest("type4-1.in"));
  EXPECT_FALSE(endToEndTest("type4-2.in"));
  EXPECT_FALSE(endToEndTest("type4-3.in"));
}

TEST(AnnotatorTest, Type5) {
  EXPECT_TRUE(endToEndTest("type5-1.in"));
  EXPECT_FALSE(endToEndTest("type5-2.in"));
}

TEST(AnnotatorTest, Type6) { EXPECT_TRUE(endToEndTest("type6.in")); }

TEST(AnnotatorTest, Type7) { EXPECT_FALSE(endToEndTest("type7.in")); }

TEST(AnnotatorTest, Type8) {
  EXPECT_TRUE(endToEndTest("type8-1.in"));
  EXPECT_FALSE(endToEndTest("type8-2.in"));
}
#include "Lexer.hpp"
#include "gtest/gtest.h"
#include <fstream>
#include <iostream>
#include <sstream>

const std::string test_cases_dir = TEST_CASES_DIR;

auto readFileToString(const std::string &filepath) -> std::string {
  // 1. 创建一个文件输入流对象
  std::ifstream file_stream(filepath);
  // 2. 检查文件是否成功打开
  if (!file_stream.is_open()) {
    // 在实际应用中，这里可能需要打印错误或抛出异常
    std::cerr << "Error: Could not open file " << filepath << std::endl;
    return "";
  }
  // 3. 创建一个字符串流
  std::stringstream buffer;
  // 4. 将文件流的缓冲区内容一次性读入字符串流
  buffer << file_stream.rdbuf();
  // 5. 从字符串流获取 std::string 并返回
  return buffer.str();
}

TEST(LexerTest, VaildToken) {
  std::string input_path = test_cases_dir + "/lexer_1.in";
  std::string text = readFileToString(input_path);
  auto lex_result = lex(text);

  std::vector<Token> stream = {
      // Keywords
      {"as", TokenType::AS},
      {"break", TokenType::BREAK},
      {"const", TokenType::CONST},
      {"continue", TokenType::CONTINUE},
      {"crate", TokenType::CRATE},
      {"else", TokenType::ELSE},
      {"enum", TokenType::ENUM},
      {"extern", TokenType::EXTERN},
      {"false", TokenType::FALSE},
      {"fn", TokenType::FN},
      {"for", TokenType::FOR},
      {"if", TokenType::IF},
      {"impl", TokenType::IMPL},
      {"in", TokenType::IN},
      {"let", TokenType::LET},
      {"loop", TokenType::LOOP},
      {"match", TokenType::MATCH},
      {"mod", TokenType::MOD},
      {"move", TokenType::MOVE},
      {"mut", TokenType::MUT},
      {"pub", TokenType::PUB},
      {"ref", TokenType::REF},
      {"return", TokenType::RETURN},
      {"self", TokenType::SELF},
      {"Self", TokenType::SELF_TYPE},
      {"static", TokenType::STATIC},
      {"struct", TokenType::STRUCT},
      {"super", TokenType::SUPER},
      {"trait", TokenType::TRAIT},
      {"true", TokenType::TRUE},
      {"type", TokenType::TYPE},
      {"unsafe", TokenType::UNSAFE},
      {"use", TokenType::USE},
      {"where", TokenType::WHERE},
      {"while", TokenType::WHILE},

      // Operators and Punctuation
      {"=", TokenType::ASSIGN},
      {"==", TokenType::EQUAL},
      {"!=", TokenType::NOT_EQUAL},
      {"<", TokenType::LESS_THAN},
      {"<=", TokenType::LESS_EQUAL},
      {">", TokenType::GREATER_THAN},
      {">=", TokenType::GREATER_EQUAL},
      {"&&", TokenType::LOGIC_AND},
      {"||", TokenType::LOGIC_OR},
      {"!", TokenType::NOT},
      {"+", TokenType::PLUS},
      {"-", TokenType::MINUS},
      {"*", TokenType::MUL},
      {"/", TokenType::DIV},
      {"%", TokenType::MOD_CAL},
      {"^", TokenType::XOR},
      {"&", TokenType::AND},
      {"|", TokenType::OR},
      {"<<", TokenType::LEFT_SHIFT},
      {">>", TokenType::RIGHT_SHIFT},
      {"+=", TokenType::PLUS_EQUAL},
      {"-=", TokenType::MINUS_EQUAL},
      {"*=", TokenType::MUL_EQUAL},
      {"/=", TokenType::DIV_EQUAL},
      {"%=", TokenType::MOD_EQUAL},
      {"^=", TokenType::XOR_EQUAL},
      {"&=", TokenType::AND_EQUAL},
      {"|=", TokenType::OR_EQUAL},
      {"<<=", TokenType::LEFT_SHIFT_EQUAL},
      {">>=", TokenType::RIGHT_SHIFT_EQUAL},
      {"@", TokenType::AT},
      {".", TokenType::DOT},
      {"..", TokenType::DOT_DOT},
      {"...", TokenType::DOT_DOT_DOT},
      {"..=", TokenType::DOT_DOT_EQUAL},
      {",", TokenType::COMMA},
      {";", TokenType::SEMICOLON},
      {":", TokenType::COLON},
      {"::", TokenType::COLON_COLON},
      {"->", TokenType::ARROW},
      {"<-", TokenType::LEFT_ARROW},
      {"=>", TokenType::FAT_ARROW},
      {"#", TokenType::HASH},
      {"$", TokenType::DOLLAR},
      {"?", TokenType::QUESTION},
      {"_", TokenType::UNDERSCORE},
      {"{", TokenType::LEFT_BRACE},
      {"}", TokenType::RIGHT_BRACE},
      {"[", TokenType::LEFT_BRACKET},
      {"]", TokenType::RIGHT_BRACKET},
      {"(", TokenType::LEFT_PAREN},
      {")", TokenType::RIGHT_PAREN},

      // Identifiers
      {"example_identifier", TokenType::IDENTIFIER},

      // Literals
      {"'a'", TokenType::CHARLITERAL},
      {"\"hello string\"", TokenType::STRINGLITERAL},
      {"r#\"raw string with \"quotes\"\"#", TokenType::RAWSTRINGLITERAL},
      {"c\"c string literal\"", TokenType::CSTRINGLITERAL},
      {"cr#\"raw c string\"#", TokenType::RAWCSTRINGLITERAL},

      // Integer Literals
      {"1234567890", TokenType::INTEGERLITERAL},
      {"0b101010", TokenType::INTEGERLITERAL},
      {"0o777", TokenType::INTEGERLITERAL},
      {"0xABCDEF", TokenType::INTEGERLITERAL},

      // Reserved Token: Still Integer.
      {"0b1012", TokenType::INTEGERLITERAL},

      {"", TokenType::END_OF_FILE},
  };

  EXPECT_TRUE(lex_result.size() == stream.size());

  for (int32_t i = 0; i < stream.size(); ++i) {
    EXPECT_EQ(lex_result[i].content, stream[i].content);
    EXPECT_EQ(lex_result[i].type, stream[i].type);
  }
}

TEST(LexerTest, COMMENT) {
  std::string input_path = test_cases_dir + "/lexer_2.in";
  std::string text = readFileToString(input_path);
  auto lex_result = lex(text);

  std::vector<Token> stream = {
      {"let", TokenType::LET},
      {"result", TokenType::IDENTIFIER},
      {"=", TokenType::ASSIGN},
      {"100", TokenType::INTEGERLITERAL},
      {";", TokenType::SEMICOLON},
      {"let", TokenType::LET},
      {"code", TokenType::IDENTIFIER},
      {"=", TokenType::ASSIGN},
      {"\"fn main() { /* not a comment */ }\"", TokenType::STRINGLITERAL},
      {";", TokenType::SEMICOLON},
      {"0x1e1f", TokenType::INTEGERLITERAL},
      {"", TokenType::END_OF_FILE},
  };

  EXPECT_TRUE(lex_result.size() == stream.size());

  for (int32_t i = 0; i < stream.size(); ++i) {
    EXPECT_EQ(lex_result[i].content, stream[i].content);
    EXPECT_EQ(lex_result[i].type, stream[i].type);
  }
}

TEST(LexerTest, STRING) {
  std::string input_path = test_cases_dir + "/lexer_3.in";
  std::string text = readFileToString(input_path);
  auto lex_result = lex(text);

  std::vector<Token> stream = {
      {"let", TokenType::LET},
      {"multi_line_str", TokenType::IDENTIFIER},
      {"=", TokenType::ASSIGN},
      {"\"line 1\\\n                          line 2\"",
       TokenType::STRINGLITERAL},
      {";", TokenType::SEMICOLON},

      {"let", TokenType::LET},
      {"whiletrue", TokenType::IDENTIFIER},
      {"=", TokenType::ASSIGN},
      {"true", TokenType::TRUE},
      {";", TokenType::SEMICOLON},
      {"let", TokenType::LET},
      {"for_loop", TokenType::IDENTIFIER},
      {"=", TokenType::ASSIGN},
      {"0", TokenType::INTEGERLITERAL},
      {";", TokenType::SEMICOLON},
      {"let", TokenType::LET},
      {"raw_string", TokenType::IDENTIFIER},
      {"=", TokenType::ASSIGN},

      {"r#\"\n    fn main() {\n        let message = \"Hello, #Rust!\";\n    "
       "}\n\"#",
       TokenType::RAWSTRINGLITERAL},
      {";", TokenType::SEMICOLON},

      {"", TokenType::END_OF_FILE},
  };

  EXPECT_TRUE(lex_result.size() == stream.size());

  for (int32_t i = 0; i < stream.size(); ++i) {
    EXPECT_EQ(lex_result[i].content, stream[i].content);
    EXPECT_EQ(lex_result[i].type, stream[i].type);
  }
}

TEST(LexerTest, COMMENT_ERROR) {
  std::string input_path = test_cases_dir + "/lexer_4.in";
  std::string text = readFileToString(input_path);
  try {
    auto lex_result = lex(text);
  } catch (std::runtime_error err) {
    std::cout << err.what() << '\n';
  }
}
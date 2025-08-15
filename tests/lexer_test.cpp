#include "lexer.hpp"
#include "gtest/gtest.h"
#include <fstream>
#include <sstream>

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
  std::string input_path = "testcases/lexer_1.in";
  std::string text = readFileToString(input_path);
  auto lex_result = lex(text);
  EXPECT_TRUE(lex_result.size() == 1);
  EXPECT_EQ(lex_result[0].type, TokenType::END_OF_FILE);
}
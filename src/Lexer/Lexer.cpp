#include "Lexer.hpp"
#include <regex>
#include <stdexcept>
#include <stdint.h>
#include <string_view>

const uint32_t MAXLENGTH = 64;

class LineFinder {
private:
  std::vector<int32_t> line_starts_;
  int32_t line_count_;
  int32_t cursor_;

public:
  LineFinder(const std::string &target) : line_count_(0), cursor_(0) {
    line_starts_.push_back(0);
    int32_t length = target.length();
    for (int32_t i = 0; i < length; ++i) {
      if (target[i] == '\n') {
        line_starts_.push_back(i + 1);
      }
    }
    if (length > 0 && target[length - 1] != '\n') {
      line_starts_.push_back(length);
    }
  }

  auto getLine() -> int32_t { return line_count_ + 1; }

  void forward(int32_t length) {
    cursor_ += length;
    while (cursor_ >= line_starts_[line_count_ + 1]) {
      ++line_count_;
    }
  }
};

struct PlainRule {
  std::string prefix;
  int32_t length;
  TokenType type;
};

struct RegexRule {
  std::regex rule;
  TokenType type;
};

const std::vector<PlainRule> config_plain_rules = {
    {"continue", 8, TokenType::CONTINUE},
    {"extern", 6, TokenType::EXTERN},
    {"return", 6, TokenType::RETURN},
    {"static", 6, TokenType::STATIC},
    {"struct", 6, TokenType::STRUCT},
    {"unsafe", 6, TokenType::UNSAFE},
    {"break", 5, TokenType::BREAK},
    {"const", 5, TokenType::CONST},
    {"crate", 5, TokenType::CRATE},
    {"false", 5, TokenType::FALSE},
    {"match", 5, TokenType::MATCH},
    {"super", 5, TokenType::SUPER},
    {"trait", 5, TokenType::TRAIT},
    {"where", 5, TokenType::WHERE},
    {"while", 5, TokenType::WHILE},
    {"else", 4, TokenType::ELSE},
    {"enum", 4, TokenType::ENUM},
    {"impl", 4, TokenType::IMPL},
    {"loop", 4, TokenType::LOOP},
    {"move", 4, TokenType::MOVE},
    {"self", 4, TokenType::SELF},
    {"Self", 4, TokenType::SELF_TYPE},
    {"true", 4, TokenType::TRUE},
    {"type", 4, TokenType::TYPE},
    {"for", 3, TokenType::FOR},
    {"let", 3, TokenType::LET},
    {"mod", 3, TokenType::MOD},
    {"mut", 3, TokenType::MUT},
    {"pub", 3, TokenType::PUB},
    {"ref", 3, TokenType::REF},
    {"use", 3, TokenType::USE},
    {"<<=", 3, TokenType::LEFT_SHIFT_EQUAL},
    {">>=", 3, TokenType::RIGHT_SHIFT_EQUAL},
    {"...", 3, TokenType::DOT_DOT_DOT},
    {"..=", 3, TokenType::DOT_DOT_EQUAL},
    {"as", 2, TokenType::AS},
    {"fn", 2, TokenType::FN},
    {"if", 2, TokenType::IF},
    {"in", 2, TokenType::IN},
    {"==", 2, TokenType::EQUAL},
    {"!=", 2, TokenType::NOT_EQUAL},
    {"<=", 2, TokenType::LESS_EQUAL},
    {">=", 2, TokenType::GREATER_EQUAL},
    {"&&", 2, TokenType::LOGIC_AND},
    {"||", 2, TokenType::LOGIC_OR},
    {"+=", 2, TokenType::PLUS_EQUAL},
    {"-=", 2, TokenType::MINUS_EQUAL},
    {"*=", 2, TokenType::MUL_EQUAL},
    {"/=", 2, TokenType::DIV_EQUAL},
    {"%=", 2, TokenType::MOD_EQUAL},
    {"^=", 2, TokenType::XOR_EQUAL},
    {"&=", 2, TokenType::AND_EQUAL},
    {"|=", 2, TokenType::OR_EQUAL},
    {"<<", 2, TokenType::LEFT_SHIFT},
    {">>", 2, TokenType::RIGHT_SHIFT},
    {"..", 2, TokenType::DOT_DOT},
    {"::", 2, TokenType::COLON_COLON},
    {"->", 2, TokenType::ARROW},
    {"<-", 2, TokenType::LEFT_ARROW},
    {"=>", 2, TokenType::FAT_ARROW},
    {"=", 1, TokenType::ASSIGN},
    {"<", 1, TokenType::LESS_THAN},
    {">", 1, TokenType::GREATER_THAN},
    {"!", 1, TokenType::NOT},
    {"+", 1, TokenType::PLUS},
    {"-", 1, TokenType::MINUS},
    {"*", 1, TokenType::MUL},
    {"/", 1, TokenType::DIV},
    {"%", 1, TokenType::MOD_CAL},
    {"^", 1, TokenType::XOR},
    {"&", 1, TokenType::AND},
    {"|", 1, TokenType::OR},
    {"@", 1, TokenType::AT},
    {".", 1, TokenType::DOT},
    {",", 1, TokenType::COMMA},
    {";", 1, TokenType::SEMICOLON},
    {":", 1, TokenType::COLON},
    {"#", 1, TokenType::HASH},
    {"$", 1, TokenType::DOLLAR},
    {"?", 1, TokenType::QUESTION},
    {"_", 1, TokenType::UNDERSCORE},
    {"{", 1, TokenType::LEFT_BRACE},
    {"}", 1, TokenType::RIGHT_BRACE},
    {"[", 1, TokenType::LEFT_BRACKET},
    {"]", 1, TokenType::RIGHT_BRACKET},
    {"(", 1, TokenType::LEFT_PAREN},
    {")", 1, TokenType::RIGHT_PAREN},
};

const std::vector<RegexRule> config_regex_rules = {
    {std::regex(R"(^[a-zA-Z][a-zA-Z0-9_]*)"), TokenType::IDENTIFIER},

    {std::regex(R"(^'([^'\\\n\r\t]|\\x[0-7][0-9a-fA-F]|\\[nrt0'\\])')"),
     TokenType::CHARLITERAL},

    {std::regex(R"(^"([^"\\\r]|\\x[0-7][0-9a-fA-F]|\\[nrt0"\\]|\\\n)*")"),
     TokenType::STRINGLITERAL},
    {std::regex(R"(^r(#*)\"[^\r]*?\"\1)"), TokenType::RAWSTRINGLITERAL},

    {std::regex(
         R"(^c"([^"\\\r\x00]|\\x(?!00)[0-7][0-9a-fA-F]|\\[nrt"\\]|\\\n)*")"),
     TokenType::CSTRINGLITERAL},
    {std::regex(R"(^cr(#*)\"[^\r\x00]*?\"\1)"), TokenType::RAWCSTRINGLITERAL},

    {std::regex(
         R"(^([0-9][0-9_]*|0b[0-1_]*[0-1][0-1_]*|0o[0-7_]*[0-7][0-7_]*|0x[0-9a-fA-F_]*[0-9a-fA-F][0-9a-fA-F_]*)([a-df-zA-DF-Z][a-zA-Z0-9_]*)?)"),
     TokenType::INTEGERLITERAL},

    {std::regex(R"(^\s+)"), TokenType::WHITESPACE},
};

class Matcher {
private:
  std::vector<PlainRule> plain_rules;
  std::vector<RegexRule> regex_rules;

  Matcher();
  auto lexRegex(const std::string &target) const -> Token;
  auto lexPlain(const std::string &target) const -> Token;

public:
  static auto getInstance() -> Matcher &;
  auto lexString(const std::string &target) const -> Token;
};

Matcher::Matcher() {
  for (int32_t i = 0; i < config_plain_rules.size(); ++i) {
    plain_rules.push_back(config_plain_rules[i]);
  }
  for (int32_t i = 0; i < config_regex_rules.size(); ++i) {
    regex_rules.push_back(config_regex_rules[i]);
  }
}

auto Matcher::getInstance() -> Matcher & {
  static Matcher instance;
  return instance;
}

auto Matcher::lexString(const std::string &target) const -> Token {
  int32_t length = 0;
  Token plain_result;
  Token regex_result;
  plain_result = lexPlain(target);
  regex_result = lexRegex(target);
  if (plain_result.content.length() < regex_result.content.length()) {
    return regex_result;
  }
  if (plain_result.content.length() == 0) {
    throw std::runtime_error("Lexer error: no match rules.");
  }
  return plain_result;
}

auto Matcher::lexPlain(const std::string &target) const -> Token {
  int32_t max_length = 0;
  Token result;
  std::string_view str_view(target);
  for (auto iter = plain_rules.begin(); iter != plain_rules.end(); ++iter) {
    /*The first prefix matched is the longest in all plain_prefixes.*/
    if (str_view.length() >= iter->length &&
        str_view.substr(0, iter->length) == iter->prefix) {
      result.content = iter->prefix;
      result.type = iter->type;
      break;
    }
  }
  return result;
}

auto Matcher::lexRegex(const std::string &target) const -> Token {
  int32_t max_length = 0;
  Token result;
  std::smatch match;
  auto search_begin = target.begin();
  auto search_end = search_begin + MAXLENGTH;
  for (auto iter = regex_rules.begin(); iter != regex_rules.end(); ++iter) {
    if (std::regex_search(search_begin, search_end, match, iter->rule) &&
        match[0].str().length() > max_length) {
      max_length = match[0].str().length();
      result.content = match[0].str();
      result.type = iter->type;
    }
  }
  return result;
}

auto commentLex(const std::string &target) -> std::string {
  enum class CtxStatus { CODE, STRING, BLOCKCOMMENT, LINECOMMENT };
  CtxStatus status = CtxStatus::CODE;
  std::string result = target;
  int32_t str_length = target.length();
  int32_t block_count = 0;
  for (int32_t i = 0; i != str_length; ++i) {
    switch (status) {
    case CtxStatus::CODE:
      if (result[i] == '\"') {
        status = CtxStatus::STRING;
      } else if (i + 1 < str_length && result[i] == '/' &&
                 result[i + 1] == '*') {
        status = CtxStatus::BLOCKCOMMENT;
        result[i] = result[i + 1] = ' ';
        ++i;
        block_count = 1;
      } else if (i + 1 < str_length && result[i] == '/' &&
                 result[i + 1] == '/') {
        status = CtxStatus::LINECOMMENT;
        result[i] = result[i + 1] = ' ';
        ++i;
      }
      break;
    case CtxStatus::STRING:
      if (i + 1 < str_length && result[i] == '\\') {
        ++i;
      } else if (result[i] == '\"') {
        status = CtxStatus::CODE;
      }
      break;
    case CtxStatus::BLOCKCOMMENT:
      if (i + 1 < str_length && result[i] == '/' && result[i + 1] == '*') {
        ++block_count;
        result[i] = ' ';
        ++i;
      } else if (i + 1 < str_length && result[i] == '*' &&
                 result[i + 1] == '/') {
        --block_count;
        result[i] = ' ';
        ++i;
        status = block_count == 0 ? CtxStatus::CODE : CtxStatus::BLOCKCOMMENT;
      }
      result[i] = result[i] == '\n' ? '\n' : ' ';
      break;
    case CtxStatus::LINECOMMENT:
      if (result[i] == '\n') {
        status = CtxStatus::CODE;
      } else {
        result[i] = ' ';
      }
      break;
    };
  }
  if (status != CtxStatus::CODE) {
    std::string error;
    switch (status) {
    case CtxStatus::BLOCKCOMMENT:
      error = "A block comment has not closed";
      break;
    case CtxStatus::LINECOMMENT:
      error = "A line comment has not closed";
      break;
    case CtxStatus::STRING:
      error = "A string has not ended.";
      break;
    }
    throw std::runtime_error(error);
  }
  return result;
}

auto lex(const std::string &target) -> std::vector<Token> {
  LineFinder line_finder_(target);
  std::vector<Token> result;
  std::string buffer = commentLex(target);
  while (!buffer.empty()) {
    auto next_token = Matcher::getInstance().lexString(buffer);
    auto next_length = next_token.content.length();
    next_token.line.setLine(line_finder_.getLine());
    line_finder_.forward(next_length);
    buffer.erase(0, next_length);
    if (next_token.type != TokenType::WHITESPACE) {
      result.push_back(next_token);
    }
  }
  result.push_back(Token{"", TokenType::END_OF_FILE});
  return result;
}
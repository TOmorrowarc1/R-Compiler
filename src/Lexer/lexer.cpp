#include "lexer.hpp"
#include <regex>
#include <stdexcept>
#include <stdint.h>

struct Rule {
  std::regex rule;
  TokenType type;
};

class Matcher {
private:
  std::vector<Rule> rules;
  Matcher() = default;

public:
  static auto getInstance() -> Matcher &;
  void loadRule(const std::string &rule, TokenType type);
  auto lexString(const std::string &target) const -> Token;
};

auto Matcher::getInstance() -> Matcher & {
  static Matcher instance;
  return instance;
}

void Matcher::loadRule(const std::string &rule, TokenType type) {
  std::string front_rule = '^' + rule;
  rules.push_back({std::regex(front_rule), type});
}

auto Matcher::lexString(const std::string &target) const -> Token {
  int32_t max_length = 0;
  Token result;
  std::smatch match;
  for (auto iter = rules.begin(); iter != rules.end(); ++iter) {
    if (std::regex_search(target, match, iter->rule) &&
        match[0].str().length() > max_length) {
      max_length = match[0].str().length();
      result.content = match[0].str();
      result.type = iter->type;
    }
  }
  if (result.content.empty()) {
    throw std::runtime_error("Lexer error: no match regex.");
  }
  return result;
}

auto lex(const std::string &target) -> std::vector<Token> {
  std::vector<Token> result;
  std::string buffer = target;
  while (!buffer.empty()) {
    auto next_token = Matcher::getInstance().lexString(buffer);
    result.push_back(next_token);
    buffer.erase(0, next_token.content.length());
  }
  return result;
}
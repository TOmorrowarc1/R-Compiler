#include "lexer.hpp"
#include <regex>
#include <stdexcept>
#include <stdint.h>

struct Rule {
  std::regex rule;
  TokenType type;
};

const int32_t RULES_NUM = 101;

const Rule config_rules[RULES_NUM] = {
    /*
    The list for Lexer gramma, in which all keywords and punctions were done by
    Gemini 2.5 Pro while others done by hands.
    Undone: suffix, comment, merge.
    */
    {std::regex(R"(^as)"), TokenType::AS},
    {std::regex(R"(^break)"), TokenType::BREAK},
    {std::regex(R"(^const)"), TokenType::CONST},
    {std::regex(R"(^continue)"), TokenType::CONTINUE},
    {std::regex(R"(^crate)"), TokenType::CRATE},
    {std::regex(R"(^else)"), TokenType::ELSE},
    {std::regex(R"(^enum)"), TokenType::ENUM},
    {std::regex(R"(^extern)"), TokenType::EXTERN},
    {std::regex(R"(^false)"), TokenType::FALSE},
    {std::regex(R"(^fn)"), TokenType::FN},
    {std::regex(R"(^for)"), TokenType::FOR},
    {std::regex(R"(^if)"), TokenType::IF},
    {std::regex(R"(^impl)"), TokenType::IMPL},
    {std::regex(R"(^in)"), TokenType::IN},
    {std::regex(R"(^let)"), TokenType::LET},
    {std::regex(R"(^loop)"), TokenType::LOOP},
    {std::regex(R"(^match)"), TokenType::MATCH},
    {std::regex(R"(^mod)"), TokenType::MOD},
    {std::regex(R"(^move)"), TokenType::MOVE},
    {std::regex(R"(^mut)"), TokenType::MUT},
    {std::regex(R"(^pub)"), TokenType::PUB},
    {std::regex(R"(^ref)"), TokenType::REF},
    {std::regex(R"(^return)"), TokenType::RETURN},
    {std::regex(R"(^self)"), TokenType::SELF},
    {std::regex(R"(^Self)"), TokenType::SELF_TYPE}, // 大写的 Self
    {std::regex(R"(^static)"), TokenType::STATIC},
    {std::regex(R"(^struct)"), TokenType::STRUCT},
    {std::regex(R"(^super)"), TokenType::SUPER},
    {std::regex(R"(^trait)"), TokenType::TRAIT},
    {std::regex(R"(^true)"), TokenType::TRUE},
    {std::regex(R"(^type)"), TokenType::TYPE},
    {std::regex(R"(^unsafe)"), TokenType::UNSAFE},
    {std::regex(R"(^use)"), TokenType::USE},
    {std::regex(R"(^where)"), TokenType::WHERE},
    {std::regex(R"(^while)"), TokenType::WHILE},

    {std::regex(R"(^=)"), TokenType::ASSIGN},
    {std::regex(R"(^==)"), TokenType::EQUAL},
    {std::regex(R"(^!=)"), TokenType::NOT_EQUAL},
    {std::regex(R"(^<)"), TokenType::LESS_THAN},
    {std::regex(R"(^<=)"), TokenType::LESS_EQUAL},
    {std::regex(R"(^>)"), TokenType::GREATER_THAN},
    {std::regex(R"(^>=)"), TokenType::GREATER_EQUAL},

    {std::regex(R"(^&&)"), TokenType::LOGIC_AND},
    {std::regex(R"(^\|\|)"), TokenType::LOGIC_OR},
    {std::regex(R"(^!)"), TokenType::NOT},

    {std::regex(R"(^\+)"), TokenType::PLUS},
    {std::regex(R"(^-)"), TokenType::MINUS},
    {std::regex(R"(^\*)"), TokenType::MUL},
    {std::regex(R"(^/)"), TokenType::DIV},
    {std::regex(R"(^%)"), TokenType::MOD_CAL},
    {std::regex(R"(^\^)"), TokenType::XOR},
    {std::regex(R"(^&)"), TokenType::AND},
    {std::regex(R"(^\|)"), TokenType::OR},
    {std::regex(R"(^<<)"), TokenType::LEFT_SHIFT},
    {std::regex(R"(^>>)"), TokenType::RIGHT_SHIFT},

    {std::regex(R"(^\+=)"), TokenType::PLUS_EQUAL},
    {std::regex(R"(^-=)"), TokenType::MINUS_EQUAL},
    {std::regex(R"(^\*=)"), TokenType::MUL_EQUAL},
    {std::regex(R"(^/=)"), TokenType::DIV_EQUAL},
    {std::regex(R"(^%=)"), TokenType::MOD_EQUAL},
    {std::regex(R"(^\^=)"), TokenType::XOR_EQUAL},
    {std::regex(R"(^&=)"), TokenType::AND_EQUAL},
    {std::regex(R"(^\|=)"), TokenType::OR_EQUAL},
    {std::regex(R"(^<<=)"), TokenType::LEFT_SHIFT_EQUAL},
    {std::regex(R"(^>>=)"), TokenType::RIGHT_SHIFT_EQUAL},

    {std::regex(R"(^@)"), TokenType::AT},
    {std::regex(R"(^\.)"), TokenType::DOT},
    {std::regex(R"(^\.\.)"), TokenType::DOT_DOT},
    {std::regex(R"(^\.\.\.)"), TokenType::DOT_DOT_DOT},
    {std::regex(R"(^\.\.=)"), TokenType::DOT_DOT_EQUAL},

    {std::regex(R"(^,)"), TokenType::COMMA},
    {std::regex(R"(^;)"), TokenType::SEMICOLON},
    {std::regex(R"(^:)"), TokenType::COLON},
    {std::regex(R"(^::)"), TokenType::COLON_COLON},

    {std::regex(R"(^->)"), TokenType::ARROW},
    {std::regex(R"(^<-)"), TokenType::LEFT_ARROW},
    {std::regex(R"(^=>)"), TokenType::FAT_ARROW},

    {std::regex(R"(^#)"), TokenType::HASH},
    {std::regex(R"(^\$)"), TokenType::DOLLAR},
    {std::regex(R"(^\?)"), TokenType::QUESTION},
    {std::regex(R"(^_)"), TokenType::UNDERSCORE},

    {std::regex(R"(^\{)"), TokenType::LEFT_BRACE},
    {std::regex(R"(^\})"), TokenType::RIGHT_BRACE},
    {std::regex(R"(^\[)"), TokenType::LEFT_BRACKET},
    {std::regex(R"(^\])"), TokenType::RIGHT_BRACKET},
    {std::regex(R"(^\()"), TokenType::LEFT_PAREN},
    {std::regex(R"(^\))"), TokenType::RIGHT_PAREN},

    {std::regex(R"(^[a-zA-Z][a-zA-Z0-9_]*)"), TokenType::IDENTIFIER},
    {std::regex(R"(^r#(?!(crate|self|super|Self)\b)[a-zA-Z][a-zA-Z0-9_]*)"),
     TokenType::RAWIDENTIFIER},

    {std::regex(R"(^'([^'\\\n\r\t]|\\x[0-7][0-9a-fA-F]|\\[nrt0'\\])')"),
     TokenType::CHARLITERAL},

    {std::regex(R"(^"([^"\\\r]|\\x[0-7][0-9a-fA-F]|\\[nrt0"\\]|\\\n)*")"),
     TokenType::STRINGLITERAL},
    {std::regex(R"(^r(#*)\".*?\"\1)"), TokenType::RAWSTRINGLITERAL},

    {std::regex(R"(^b'([^'\\\r]|\\x[0-7][0-9a-fA-F]|\\[nrt0'\\])*")"),
     TokenType::BYTELITERAL},
    {std::regex(R"(^b"([^"\\\r]|\\x[0-7][0-9a-fA-F]|\\[nrt0"\\]|\\\n)*")"),
     TokenType::BYTESTRINGLITERAL},
    {std::regex(R"(^br(#*)\".*?\"\1)"), TokenType::RAWBYTESTRINGLITERAL},

    {std::regex(
         R"(^c"([^"\\\r\x00]|\\x(?!00)[0-7][0-9a-fA-F]|\\[nrt"\\]|\\\n)*")"),
     TokenType::CSTRINGLITERAL},
    {std::regex(R"(^cr(#*)\"[^\r\x00]*?\"\1)"), TokenType::RAWCSTRINGLITERAL},

    {std::regex(
         R"(^([0-9][0-9_]*|0b[0-1_]*[0-1][0-1_]*|0o[0-7_]*[0-7][0-7_]*|0x[0-9a-fA-F_]*[0-9a-fA-F][0-9a-fA-F_]*)([a-df-zA-DF-Z][a-zA-Z0-9_]*)?)"),
     TokenType::INTEGERLITERAL},

    // Float may have problem on "1."
    {std::regex(R"(^[0-9][0-9_]*\.[0-9][0-9_])"), TokenType::FLOATLITERAL},

    {std::regex(R"(^\s+)"), TokenType::WHITESPACE},

    {std::regex(
         R"(^0b[0-1_]*[0-1][0-1_]*[2-9]|0o[0-7_]*[0-7][0-7_]*[8-9]|(0b[0-1_]*[0-1][0-1_]*|0o[0-7_]*[0-7][0-7_]*|0x[0-9a-fA-F_]*[0-9a-fA-F][0-9a-fA-F_]*)\.[^_\.a-zA-Z])"),
     TokenType::RESERVED},
    /*End here.*/
};

class Matcher {
private:
  std::vector<Rule> rules;
  Matcher();

public:
  static auto getInstance() -> Matcher &;
  auto lexString(const std::string &target) const -> Token;
};

Matcher::Matcher() {
  for (int32_t i = 0; i < RULES_NUM; ++i) {
    rules.push_back(config_rules[i]);
  }
}

auto Matcher::getInstance() -> Matcher & {
  static Matcher instance;
  return instance;
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
  return result;
}

auto lex(const std::string &target) -> std::vector<Token> {
  std::vector<Token> result;
  std::string buffer = commentLex(target);
  while (!buffer.empty()) {
    auto next_token = Matcher::getInstance().lexString(buffer);
    if (next_token.type == TokenType::RESERVED) {
      throw std::runtime_error("The token reached some reserved cases.");
    }
    if (next_token.type != TokenType::WHITESPACE) {
      result.push_back(next_token);
    }
    buffer.erase(0, next_token.content.length());
  }
  result.push_back(Token{"", TokenType::END_OF_FILE});
  return result;
}
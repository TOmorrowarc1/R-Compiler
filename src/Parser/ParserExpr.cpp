#include "ParserTotal.hpp"
#include "Token.hpp"
#include "cast.hpp"
#include "exception.hpp"
#include <string>
#include <unordered_map>

/*
The main logic of patt parser: split expr to nud + leds to eliminate left
recursion, and parse expr with precedence by attaching the "bind power" to
each operator.
Our Parse() function will parse the nud first and then parse the leds in a
loop until the next operation is less powerful than the current one, which
show the end of the expression. In each loop it combines original expression
with the operator and the right operand.
*/

struct bindPower {
  TokenType type;
  int32_t left_power = 0;
  int32_t right_power = 0;
};

/*
These numbers are generrated by gemini for rules are too many,
 and I haved checked them.
 */

const std::vector<bindPower> nud_powers = {
    {TokenType::MINUS, 0, 23},
    {TokenType::NOT, 0, 23},
    {TokenType::AND, 0, 23},
    {TokenType::MUL, 0, 23},
};

const std::vector<bindPower> led_powers = {
    {TokenType::ASSIGN, 2, 1},
    {TokenType::PLUS_EQUAL, 2, 1},
    {TokenType::MINUS_EQUAL, 2, 1},
    {TokenType::MUL_EQUAL, 2, 1},
    {TokenType::DIV_EQUAL, 2, 1},
    {TokenType::MOD_EQUAL, 2, 1},
    {TokenType::XOR_EQUAL, 2, 1},
    {TokenType::AND_EQUAL, 2, 1},
    {TokenType::OR_EQUAL, 2, 1},
    {TokenType::LEFT_SHIFT_EQUAL, 2, 1},
    {TokenType::RIGHT_SHIFT_EQUAL, 2, 1},

    {TokenType::LOGIC_OR, 3, 4},
    {TokenType::LOGIC_AND, 5, 6},

    {TokenType::EQUAL, 7, 7},
    {TokenType::NOT_EQUAL, 7, 7},
    {TokenType::LESS_THAN, 7, 7},
    {TokenType::LESS_EQUAL, 7, 7},
    {TokenType::GREATER_THAN, 7, 7},
    {TokenType::GREATER_EQUAL, 7, 7},

    {TokenType::OR, 9, 10},
    {TokenType::XOR, 11, 12},
    {TokenType::AND, 13, 14},

    {TokenType::LEFT_SHIFT, 15, 16},
    {TokenType::RIGHT_SHIFT, 15, 16},

    {TokenType::PLUS, 17, 18},
    {TokenType::MINUS, 17, 18},

    {TokenType::MUL, 19, 20},
    {TokenType::DIV, 19, 20},
    {TokenType::MOD_CAL, 19, 20},

    {TokenType::AS, 21, 22},

    {TokenType::LEFT_PAREN, 0, 0},
    {TokenType::LEFT_BRACKET, 21, 0},
    {TokenType::LEFT_BRACE, 1, 0},
    {TokenType::SEMICOLON, 0, 0},
};

class OpPowerRecoder {
private:
  std::unordered_map<TokenType, bindPower> nud_power_map;
  std::unordered_map<TokenType, bindPower> led_power_map;
  OpPowerRecoder() {
    for (const auto &bp : nud_powers) {
      nud_power_map[bp.type] = bp;
    }
    for (const auto &bp : led_powers) {
      led_power_map[bp.type] = bp;
    }
  }

public:
  static auto getInstance() -> OpPowerRecoder & {
    static OpPowerRecoder instance;
    return instance;
  }

  auto getLeftLed(TokenType type) const -> int32_t {
    auto iter = led_power_map.find(type);
    if (iter != led_power_map.end()) {
      return iter->second.left_power;
    }
    throw std::runtime_error("This token is not a led operator.");
    return 0;
  }

  auto getRightLed(TokenType type) const -> int32_t {
    auto iter = led_power_map.find(type);
    if (iter != led_power_map.end()) {
      return iter->second.right_power;
    }
    throw std::runtime_error("This token is not a led operator.");
    return 0;
  }

  auto getLeftNud(TokenType type) const -> int32_t {
    auto iter = nud_power_map.find(type);
    if (iter != nud_power_map.end()) {
      return iter->second.left_power;
    }
    throw std::runtime_error("This token is not a led operator.");
    return 0;
  }

  auto getRightNud(TokenType type) const -> int32_t {
    auto iter = nud_power_map.find(type);
    if (iter != nud_power_map.end()) {
      return iter->second.right_power;
    }
    throw std::runtime_error("This token is not a led operator.");
    return 0;
  }
};

auto parseExprNode(TokenStream &stream, int32_t power)
    -> std::unique_ptr<ExprNode>;

auto parseNudExprNode(TokenStream &stream, int32_t power)
    -> std::unique_ptr<ExprNode>;
auto parseLedExprNode(TokenStream &stream, const Token &token,
                      std::unique_ptr<ExprNode> &&lhs)
    -> std::unique_ptr<ExprNode>;

auto parseExprBlockInNode(TokenStream &stream)
    -> std::unique_ptr<ExprBlockInNode>;
auto parseExprBlockNode(TokenStream &stream) -> std::unique_ptr<ExprBlockNode>;
auto parseExprBlockConstNode(TokenStream &stream)
    -> std::unique_ptr<ExprBlockConstNode>;
auto parseExprLoopNode(TokenStream &stream) -> std::unique_ptr<ExprLoopNode>;
auto parseExprWhileNode(TokenStream &stream) -> std::unique_ptr<ExprWhileNode>;
auto parseExprIfNode(TokenStream &stream) -> std::unique_ptr<ExprIfNode>;

auto parseExprLiteralNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralNode>;
auto parseExprLiteralIntNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralIntNode>;
auto parseExprLiteralBoolNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralBoolNode>;
auto parseExprLiteralCharNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralCharNode>;
auto parseExprLiteralStringNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralStringNode>;

auto parseExprOperUnaryNode(TokenStream &stream)
    -> std::unique_ptr<ExprOperUnaryNode>;
auto parseExprPathNode(TokenStream &stream) -> std::unique_ptr<ExprPathNode>;
auto parseExprGroupNode(TokenStream &stream) -> std::unique_ptr<ExprGroupNode>;
auto parseExprCallNode(TokenStream &stream) -> std::unique_ptr<ExprCallNode>;
auto parseExprBreakNode(TokenStream &stream) -> std::unique_ptr<ExprBreakNode>;
auto parseExprReturnNode(TokenStream &stream)
    -> std::unique_ptr<ExprReturnNode>;
auto parseExprContinueNode(TokenStream &stream)
    -> std::unique_ptr<ExprContinueNode>;
auto parseExprArrayNode(TokenStream &stream) -> std::unique_ptr<ExprArrayNode>;

auto parseStructField(TokenStream &stream) -> ExprStructField;
auto tokenToBinaryOp(TokenType type) -> BinaryOperator;
auto parseCondition(TokenStream &stream) -> std::unique_ptr<ExprNode>;

auto parseExprNode(TokenStream &stream) -> std::unique_ptr<ExprNode> {
  return parseExprNode(stream, 0);
}

auto parseExprNode(TokenStream &stream, int32_t power)
    -> std::unique_ptr<ExprNode> {
  auto result = parseNudExprNode(stream, power);
  while (true) {
    auto token = stream.peek();
    if (token.type == TokenType::END_OF_FILE) {
      break;
    }
    int32_t left_power = OpPowerRecoder::getInstance().getLeftLed(token.type);
    if (left_power <= power) {
      break;
    }
    stream.next();
    result = parseLedExprNode(stream, token, std::move(result));
  }
  return result;
}

auto parseNudExprNode(TokenStream &stream, int32_t power)
    -> std::unique_ptr<ExprNode> {
  // It strikes me that Nud nodes are able to be LL(1) parsed.
  Position position = stream.peek().line;
  switch (stream.peek().type) {
  case TokenType::IDENTIFIER:
    return parseExprPathNode(stream);
  case TokenType::CHARLITERAL:
    return parseExprLiteralCharNode(stream);
  case TokenType::STRINGLITERAL:
  case TokenType::RAWSTRINGLITERAL:
  case TokenType::CSTRINGLITERAL:
  case TokenType::RAWCSTRINGLITERAL:
    return parseExprLiteralStringNode(stream);
  case TokenType::INTEGERLITERAL:
    return parseExprLiteralIntNode(stream);
  case TokenType::TRUE:
  case TokenType::FALSE:
    return parseExprLiteralBoolNode(stream);
  case TokenType::BREAK:
    return parseExprBreakNode(stream);
  case TokenType::CONTINUE:
    return parseExprContinueNode(stream);
  case TokenType::RETURN:
    return parseExprReturnNode(stream);
  case TokenType::MINUS:
  case TokenType::NOT:
  case TokenType::AND:
    return parseExprOperUnaryNode(stream);
  case TokenType::LEFT_PAREN: {
    stream.next();
    std::unique_ptr<ExprNode> expr;
    expr = parseExprNode(stream);
    if (stream.peek().type != TokenType::RIGHT_PAREN) {
      throw CompilerException("Expected ) after expression.", position);
    }
    stream.next();
    return std::make_unique<ExprGroupNode>(std::move(expr), position);
  }
  case TokenType::LEFT_BRACKET:
    return parseExprArrayNode(stream);
  }
  throw CompilerException("Unexpected token in expression.", position);
  return nullptr;
}

auto parseLedExprNode(TokenStream &stream, const Token &token,
                      std::unique_ptr<ExprNode> &&lhs)
    -> std::unique_ptr<ExprNode> {
  Position position = token.line;
  switch (token.type) {
  case TokenType::LEFT_PAREN: {
    std::vector<std::unique_ptr<ExprNode>> parameters;
    if (stream.peek().type != TokenType::RIGHT_PAREN) {
      parameters.push_back(parseExprNode(stream));
    }
    while (stream.peek().type == TokenType::COMMA) {
      stream.next();
      if (stream.peek().type == TokenType::RIGHT_PAREN) {
        break;
      }
      parameters.push_back(parseExprNode(stream));
    }
    if (stream.peek().type != TokenType::RIGHT_PAREN) {
      throw CompilerException("Missing ) after parameters.", position);
    }
    stream.next();
    return std::make_unique<ExprCallNode>(std::move(lhs), std::move(parameters),
                                          position);
  }
  case TokenType::LEFT_BRACKET: {
    auto index = parseExprNode(stream);
    if (stream.peek().type != TokenType::RIGHT_BRACKET) {
      throw CompilerException("Lost ] after an index.", position);
    }
    stream.next();
    return std::make_unique<ExprArrayIndexNode>(std::move(lhs),
                                                std::move(index), position);
  }
  case TokenType::LEFT_BRACE: {
    std::vector<ExprStructField> fields;
    if (stream.peek().type != TokenType::RIGHT_BRACE) {
      fields.push_back(parseStructField(stream));
    }
    while (stream.peek().type == TokenType::COMMA) {
      stream.next();
      if (stream.peek().type == TokenType::RIGHT_BRACE) {
        break;
      }
      fields.push_back(parseStructField(stream));
    }
    if (stream.peek().type != TokenType::RIGHT_BRACE) {
      throw CompilerException("Missing } in struct fields.", position);
    }
    stream.next();
    auto expr_path = dynamic_unique_ptr_cast<ExprPathNode, ExprNode>(lhs);
    if (expr_path == nullptr) {
      throw CompilerException("The expr in struct is not a path.", position);
    }
    return std::make_unique<ExprStructNode>(std::move(expr_path),
                                            std::move(fields), position);
  }
  case TokenType::DOT: {
    auto next_token = stream.next();
    if (next_token.type != TokenType::IDENTIFIER) {
      throw CompilerException("Except id after the dot.", position);
    }
    std::string ID = next_token.content;
    if (stream.peek().type == TokenType::LEFT_PAREN) {
      stream.next();
      std::vector<std::unique_ptr<ExprNode>> parameters;
      if (stream.peek().type != TokenType::RIGHT_PAREN) {
        parameters.push_back(parseExprNode(stream));
      }
      while (stream.peek().type == TokenType::COMMA) {
        stream.next();
        if (stream.peek().type == TokenType::RIGHT_PAREN) {
          break;
        }
        parameters.push_back(parseExprNode(stream));
      }
      if (stream.peek().type != TokenType::RIGHT_PAREN) {
        throw CompilerException("Missing ) in struct fields.", position);
      }
      stream.next();
      return std::make_unique<ExprMethodNode>(std::move(lhs), ID,
                                              std::move(parameters), position);
    }
    return std::make_unique<ExprFieldNode>(std::move(lhs), ID, position);
  }
  }
  int32_t right_power = OpPowerRecoder::getInstance().getRightLed(token.type);
  auto rhs = parseExprNode(stream, right_power);
  return std::make_unique<ExprOperBinaryNode>(
      tokenToBinaryOp(token.type), std::move(lhs), std::move(rhs), position);
}

auto tokenToBinaryOp(TokenType type) -> BinaryOperator {
  // By Gemini.
  switch (type) {
  case TokenType::ASSIGN:
    return BinaryOperator::ASSIGN;
  case TokenType::AS:
    return BinaryOperator::AS_CAST;
  case TokenType::EQUAL:
    return BinaryOperator::EQUAL;
  case TokenType::NOT_EQUAL:
    return BinaryOperator::NOT_EQUAL;
  case TokenType::LESS_THAN:
    return BinaryOperator::LESS_THAN;
  case TokenType::LESS_EQUAL:
    return BinaryOperator::LESS_EQUAL;
  case TokenType::GREATER_THAN:
    return BinaryOperator::GREATER_THAN;
  case TokenType::GREATER_EQUAL:
    return BinaryOperator::GREATER_EQUAL;
  case TokenType::LOGIC_AND:
    return BinaryOperator::LOGIC_AND;
  case TokenType::LOGIC_OR:
    return BinaryOperator::LOGIC_OR;
  case TokenType::PLUS:
    return BinaryOperator::PLUS;
  case TokenType::MINUS:
    return BinaryOperator::MINUS;
  case TokenType::MUL:
    return BinaryOperator::MUL;
  case TokenType::DIV:
    return BinaryOperator::DIV;
  case TokenType::MOD_CAL:
    return BinaryOperator::MOD;
  case TokenType::XOR:
    return BinaryOperator::XOR;
  case TokenType::AND:
    return BinaryOperator::AND;
  case TokenType::OR:
    return BinaryOperator::OR;
  case TokenType::LEFT_SHIFT:
    return BinaryOperator::LEFT_SHIFT;
  case TokenType::RIGHT_SHIFT:
    return BinaryOperator::RIGHT_SHIFT;
  case TokenType::PLUS_EQUAL:
    return BinaryOperator::PLUS_EQUAL;
  case TokenType::MINUS_EQUAL:
    return BinaryOperator::MINUS_EQUAL;
  case TokenType::MUL_EQUAL:
    return BinaryOperator::MUL_EQUAL;
  case TokenType::DIV_EQUAL:
    return BinaryOperator::DIV_EQUAL;
  case TokenType::MOD_EQUAL:
    return BinaryOperator::MOD_EQUAL;
  case TokenType::XOR_EQUAL:
    return BinaryOperator::XOR_EQUAL;
  case TokenType::AND_EQUAL:
    return BinaryOperator::AND_EQUAL;
  case TokenType::OR_EQUAL:
    return BinaryOperator::OR_EQUAL;
  case TokenType::LEFT_SHIFT_EQUAL:
    return BinaryOperator::LEFT_SHIFT_EQUAL;
  case TokenType::RIGHT_SHIFT_EQUAL:
    return BinaryOperator::RIGHT_SHIFT_EQUAL;
  }
  throw std::runtime_error("This token is not a binary operator.");
  return BinaryOperator::ASSIGN;
}

auto parseExprBlockInNode(TokenStream &stream)
    -> std::unique_ptr<ExprBlockInNode> {
  Position position = stream.peek().line;
  switch (stream.peek().type) {
  case TokenType::LEFT_BRACE:
    return parseExprBlockNode(stream);
  case TokenType::CONST:
    return parseExprBlockConstNode(stream);
  case TokenType::LOOP:
    return parseExprLoopNode(stream);
  case TokenType::WHILE:
    return parseExprWhileNode(stream);
  case TokenType::IF:
    return parseExprIfNode(stream);
  }
  throw CompilerException("Unexpected token in parseExprBlockInNode", position);
  return nullptr;
}

auto parseStructField(TokenStream &stream) -> ExprStructField {
  Position position = stream.peek().line;
  ExprStructField field;
  if (stream.peek().type != TokenType::IDENTIFIER) {
    throw CompilerException("Fields are without identifier.", position);
  }
  std::string ID = stream.next().content;
  if (stream.peek().type == TokenType::COLON) {
    stream.next();
    field.expr = parseExprNode(stream);
  }
  return field;
}

auto parseExprBlockNode(TokenStream &stream) -> std::unique_ptr<ExprBlockNode> {
  Position position = stream.peek().line;
  std::vector<std::unique_ptr<StmtNode>> statements;
  std::unique_ptr<ExprBlockOutNode> return_value;
  if (stream.next().type != TokenType::LEFT_BRACE) {
    throw CompilerException("The block expr no left brace.", position);
  }
  while (stream.peek().type != TokenType::RIGHT_BRACE) {
    if (stream.peek().type == TokenType::END_OF_FILE) {
      throw CompilerException("The block expr missed right brace.", position);
    }
    /*Here is the ugly logic of stmt& expr parse--we have to deal the "part
    recursion" of stmt and exprs.*/
    bool end_flag = false;
    switch (stream.peek().type) {
    case TokenType::SEMICOLON:
    case TokenType::LET:
    case TokenType::MOD:
    case TokenType::FN:
    case TokenType::CONST:
    case TokenType::STRUCT:
    case TokenType::ENUM:
    case TokenType::IMPL:
      statements.push_back(parseStmtNode(stream));
    default:
      auto expr = parseExprNode(stream);
      if (is_instance_of<ExprBlockOutNode, ExprNode>(expr) &&
          stream.peek().type != TokenType::SEMICOLON) {
        return_value = std::move(
            dynamic_unique_ptr_cast<ExprBlockOutNode, ExprNode>(expr));
        end_flag = true;
      } else {
        statements.push_back(
            std::make_unique<StmtExprNode>(std::move(expr), position));
      }
    }
    if (end_flag) {
      if (stream.peek().type != TokenType::RIGHT_BRACE) {
        throw CompilerException("The block expr missed right brace.", position);
      }
      break;
    }
  }
  stream.next();
  return std::make_unique<ExprBlockNode>(std::move(statements),
                                         std::move(return_value), position);
}

auto parseExprBlockConstNode(TokenStream &stream)
    -> std::unique_ptr<ExprBlockConstNode> {
  Position position = stream.peek().line;
  if (stream.next().type != TokenType::CONST) {
    throw CompilerException("the const block missed const.", position);
  }
  return std::make_unique<ExprBlockConstNode>(parseExprBlockNode(stream),
                                              position);
}

auto parseExprLoopNode(TokenStream &stream) -> std::unique_ptr<ExprLoopNode> {
  Position position = stream.peek().line;
  stream.next();
  return std::make_unique<ExprLoopNode>(parseExprBlockNode(stream), position);
}

auto parseExprWhileNode(TokenStream &stream) -> std::unique_ptr<ExprWhileNode> {
  Position position = stream.peek().line;
  stream.next();
  auto condition = parseCondition(stream);
  if (stream.next().type != TokenType::LEFT_BRACE) {
    throw CompilerException("the while expr missed left brace.", position);
  }
  auto loop_body = parseExprBlockNode(stream);
  return std::make_unique<ExprWhileNode>(std::move(condition),
                                         std::move(loop_body), position);
}

auto parseExprIfNode(TokenStream &stream) -> std::unique_ptr<ExprIfNode> {
  Position position = stream.peek().line;
  stream.next();
  auto condition = parseCondition(stream);
  if (stream.next().type != TokenType::LEFT_BRACE) {
    throw CompilerException("the if expr missed left brace.", position);
  }
  auto then_block = parseExprBlockNode(stream);
  std::unique_ptr<ExprBlockNode> else_block;
  if (stream.peek().type == TokenType::ELSE) {
    stream.next();
    if (stream.next().type != TokenType::LEFT_BRACE) {
      throw CompilerException("the else expr missed left brace.", position);
    }
    else_block = parseExprBlockNode(stream);
  }
  return std::make_unique<ExprIfNode>(std::move(condition),
                                      std::move(then_block),
                                      std::move(else_block), position);
}

auto parseCondition(TokenStream &stream) -> std::unique_ptr<ExprNode> {
  Position position = stream.peek().line;
  if (stream.peek().type != TokenType::LEFT_PAREN) {
    throw CompilerException("The condition expr missed left paren.", position);
  }
  stream.next();
  auto condition = parseExprNode(stream);
  if (stream.peek().type != TokenType::RIGHT_PAREN) {
    throw CompilerException("The condition expr missed right paren.", position);
  }
  stream.next();
  return condition;
}

auto parseExprLiteralNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralNode> {
  Position position = stream.peek().line;
  switch (stream.peek().type) {
  case TokenType::CHARLITERAL:
    return parseExprLiteralCharNode(stream);
  case TokenType::STRINGLITERAL:
  case TokenType::RAWSTRINGLITERAL:
  case TokenType::CSTRINGLITERAL:
  case TokenType::RAWCSTRINGLITERAL:
    return parseExprLiteralStringNode(stream);
  case TokenType::INTEGERLITERAL:
    return parseExprLiteralIntNode(stream);
  case TokenType::TRUE:
  case TokenType::FALSE:
    return parseExprLiteralBoolNode(stream);
  }
  throw CompilerException("Unexpected token in ExprLiteralNode: ", position);
  return nullptr;
}

auto parseExprLiteralIntNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralIntNode> {
  Position position = stream.peek().line;
  size_t end_pos = std::string::npos;
  std::string str = stream.next().content;
  bool sign = false;
  for (size_t i = 0; i < str.length() && end_pos == std::string::npos; ++i) {
    char c = str[i];
    if (std::isalpha(c)) {
      str[i] = std::tolower(c);
      if (str[i] != 'b' && str[i] != 'o' && str[i] != 'x') {
        end_pos = i;
        switch (c) {
        case 'i':
          sign = true;
          break;
        case 'u':
          sign = false;
          break;
        default:
          throw CompilerException("Unknown suffix in int literal.", position);
        }
      }
    }
  }
  std::string_view numberic_part = std::string_view(str).substr(0, end_pos);
  std::string cleaned_literal;
  cleaned_literal.reserve(numberic_part.length());
  for (char c : cleaned_literal) {
    if (c != '_') {
      cleaned_literal += c;
    }
  }
  if (cleaned_literal.empty()) {
    throw CompilerException("Literal contains no digits.", position);
  }
  int32_t base;
  size_t pos;
  if (cleaned_literal.length() > 2) {
    if (cleaned_literal[0] == '0' && cleaned_literal[1] == 'x') {
      base = 16;
      pos = 2;
    } else if (cleaned_literal[0] == '0' && cleaned_literal[1] == 'o') {
      base = 8;
      pos = 2;
    } else if (cleaned_literal[0] == '0' && cleaned_literal[1] == 'b') {
      base = 2;
      pos = 2;
    } else {
      base = 10;
      pos = 0;
    }
  }
  int32_t value = std::stoi(cleaned_literal.substr(pos), &pos, base);
  return std::make_unique<ExprLiteralIntNode>(value, sign, position);
}

auto parseExprLiteralBoolNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralBoolNode> {
  Position position = stream.peek().line;
  bool value = stream.next().type == TokenType::TRUE;
  return std::make_unique<ExprLiteralBoolNode>(value, position);
}

auto parseExprLiteralCharNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralCharNode> {
  Position position = stream.peek().line;
  std::string content = stream.next().content;
  char result = 0;
  if (content[1] == '\\') {
    switch (content[2]) {
    case '\'':
      result = '\'';
      break;
    case '"':
      result = '"';
      break;
    case 'n':
      result = '\n';
      break;
    case 'r':
      result = '\r';
      break;
    case 't':
      result = '\t';
      break;
    case '\\':
      result = '\\';
      break;
    case '0':
      result = '\0';
      break;
    case 'x': {
      size_t pos = 3;
      result = std::stoi(content, &pos, 16);
      break;
    }
    default:
      throw CompilerException("Unknown escape sequence.", position);
    }
  } else {
    result = content[1];
  }
  return std::make_unique<ExprLiteralCharNode>(static_cast<uint32_t>(result),
                                               position);
}

auto parseExprLiteralStringNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralStringNode> {
  Position position = stream.peek().line;
  std::string result;
  auto content = stream.peek().content;
  switch (stream.next().type) {
  case TokenType::STRINGLITERAL:
  case TokenType::CSTRINGLITERAL:
    result = content.substr(1, content.length() - 2);
  case TokenType::RAWSTRINGLITERAL:
  case TokenType::RAWCSTRINGLITERAL: {
    int32_t hash_count = 0;
    for (int32_t i = 1; i < content.length(); ++i) {
      if (content[i] == '#') {
        ++hash_count;
      } else if (content[i] == '\"') {
        break;
      } else {
        throw CompilerException("Raw string literal has wrong format.",
                                position);
      }
    }
    result =
        content.substr(2 + hash_count, content.length() - 3 - 2 * hash_count);
  }
  default:
    throw CompilerException("The string literal has wrong format.", position);
  }
  return std::make_unique<ExprLiteralStringNode>(std::move(result), position);
}

auto parseExprOperUnaryNode(TokenStream &stream)
    -> std::unique_ptr<ExprOperUnaryNode> {
  Position position = stream.peek().line;
  UnaryOperator op;
  switch (stream.next().type) {
  case TokenType::MINUS:
    op = UnaryOperator::NEGATE;
    break;
  case TokenType::NOT:
    op = UnaryOperator::NOT;
    break;
  case TokenType::AND:
    op = UnaryOperator::REF;
    if (stream.peek().type == TokenType::MUT) {
      stream.next();
      op = UnaryOperator::MUTREF;
    }
    break;
  case TokenType::MUL:
    op = UnaryOperator::DEREF;
    break;
  default:
    throw CompilerException("Unexpected token in unary operator.", position);
  }
  Token token = stream.next();
  auto expr = parseExprNode(
      stream, OpPowerRecoder::getInstance().getRightNud(token.type));
  return std::make_unique<ExprOperUnaryNode>(op, std::move(expr), position);
}

auto parseExprPathNode(TokenStream &stream) -> std::unique_ptr<ExprPathNode> {
  Position position = stream.peek().line;
  return std::make_unique<ExprPathNode>(parsePathNode(stream), position);
}

auto parseExprGroupNode(TokenStream &stream) -> std::unique_ptr<ExprGroupNode> {
  Position position = stream.peek().line;
  stream.next();
  auto expr = std::move(parseExprNode(stream));
  if (stream.next().type != TokenType::RIGHT_PAREN) {
    throw CompilerException("The group expr missed right parent.", position);
  }
  return std::make_unique<ExprGroupNode>(std::move(expr), position);
}

auto parseExprCallNode(TokenStream &stream) -> std::unique_ptr<ExprCallNode> {
  Position position = stream.peek().line;
  stream.next();
  auto caller = parseExprNode(stream);
  std::vector<std::unique_ptr<ExprNode>> arguments;
  if (stream.peek().type != TokenType::LEFT_PAREN) {
    throw CompilerException("The call expr missed left parent.", position);
  }
  stream.next();
  while (stream.peek().type != TokenType::RIGHT_PAREN) {
    if (stream.peek().type == TokenType::END_OF_FILE) {
      throw CompilerException("The call expr missed right parent.", position);
    }
    arguments.push_back(std::move(parseExprNode(stream)));
    if (stream.peek().type == TokenType::RIGHT_PAREN) {
      break;
    }
    if (stream.next().type != TokenType::COMMA) {
      throw CompilerException("The call expr missed comma.", position);
    }
  }
  stream.next();
  return std::make_unique<ExprCallNode>(std::move(caller), std::move(arguments),
                                        position);
}

auto parseExprBreakNode(TokenStream &stream) -> std::unique_ptr<ExprBreakNode> {
  Position position = stream.peek().line;
  stream.next();
  std::unique_ptr<ExprNode> value;
  if (stream.peek().type != TokenType::SEMICOLON) {
    value = parseExprNode(stream);
  } else {
    stream.next();
  }
  return std::make_unique<ExprBreakNode>(std::move(value), position);
}

auto parseExprReturnNode(TokenStream &stream)
    -> std::unique_ptr<ExprReturnNode> {
  Position position = stream.peek().line;
  stream.next();
  std::unique_ptr<ExprNode> value;
  if (stream.peek().type != TokenType::SEMICOLON) {
    value = parseExprNode(stream);
  } else {
    stream.next();
  }
  return std::make_unique<ExprReturnNode>(std::move(value), position);
}

auto parseExprContinueNode(TokenStream &stream)
    -> std::unique_ptr<ExprContinueNode> {
  Position position = stream.peek().line;
  stream.next();
  if (stream.peek().type != TokenType::SEMICOLON) {
    throw CompilerException("Continue expr should not have value.", position);
  }
  return std::make_unique<ExprContinueNode>(position);
}

auto parseExprArrayNode(TokenStream &stream) -> std::unique_ptr<ExprArrayNode> {
  Position position = stream.peek().line;
  stream.next();
  std::vector<std::unique_ptr<ExprNode>> elements;
  std::unique_ptr<ExprNode> length;
  if (stream.peek().type != TokenType::RIGHT_BRACKET) {
    elements.push_back(parseExprNode(stream));
    while (stream.peek().type == TokenType::COMMA) {
      stream.next();
      if (stream.peek().type == TokenType::RIGHT_BRACKET) {
        break;
      }
      elements.push_back(parseExprNode(stream));
    }
    if (stream.peek().type == TokenType::SEMICOLON) {
      stream.next();
      length = parseExprNode(stream);
    }
  }
  if (stream.peek().type != TokenType::RIGHT_BRACKET) {
    throw CompilerException("Missing ] at the end of an array.", position);
  }
  stream.next();
  return std::make_unique<ExprArrayNode>(std::move(elements), std::move(length),
                                         position);
}
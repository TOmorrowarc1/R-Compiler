#include "ParserTotal.hpp"
#include "token.hpp"
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

const std::vector<bindPower> bind_powers = {
    /*
    These numbers are generrated by gemini for rules are too many,
     and I haved checked them.
     */
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

    {TokenType::MINUS, 0, 23},
    {TokenType::NOT, 0, 23},

    {TokenType::LEFT_PAREN, 0, 0},
    {TokenType::SEMICOLON, 0, 0},
};

class OpPowerRecoder {
private:
  std::unordered_map<TokenType, bindPower> power_map;
  OpPowerRecoder() {
    for (const auto &bp : bind_powers) {
      power_map[bp.type] = bp;
    }
  }

public:
  static auto getInstance() -> OpPowerRecoder & {
    static OpPowerRecoder instance;
    return instance;
  }

  auto getLeft(TokenType type) const -> int32_t {
    auto iter = power_map.find(type);
    if (iter != power_map.end()) {
      return iter->second.left_power;
    }
    return 0;
  }

  auto getRight(TokenType type) const -> int32_t {
    auto iter = power_map.find(type);
    if (iter != power_map.end()) {
      return iter->second.right_power;
    }
    return 0;
  }
};

auto parseNudExprNode(TokenStream &stream, int32_t power)
    -> std::unique_ptr<ExprNode> {
  // It strikes me that Nud nodes are able to be LL(1) parsed.
  switch (stream.peek().type) {
  case TokenType::IDENTIFIER:
  case TokenType::RAWIDENTIFIER:
    return std::make_unique<ExprPathNode>(std::move(parseExprPathNode(stream)));
  case TokenType::CHARLITERAL:
    return std::make_unique<ExprLiteralCharNode>(
        std::move(parseExprLiteralCharNode(stream)));
  case TokenType::STRINGLITERAL:
  case TokenType::RAWSTRINGLITERAL:
  case TokenType::CSTRINGLITERAL:
  case TokenType::RAWCSTRINGLITERAL:
    return std::make_unique<ExprLiteralStringNode>(
        std::move(parseExprLiteralStringNode(stream)));
  case TokenType::INTEGERLITERAL:
    return std::make_unique<ExprLiteralIntNode>(
        std::move(parseExprLiteralIntNode(stream)));
  case TokenType::TRUE:
  case TokenType::FALSE:
    return std::make_unique<ExprLiteralBoolNode>(
        std::move(parseExprLiteralBoolNode(stream)));
  case TokenType::BREAK:
    return std::make_unique<ExprBreakNode>(
        std::move(parseExprBreakNode(stream)));
  case TokenType::CONTINUE:
    return std::make_unique<ExprContinueNode>(
        std::move(parseExprContinueNode(stream)));
  case TokenType::RETURN:
    return std::make_unique<ExprReturnNode>(
        std::move(parseExprReturnNode(stream)));
  case TokenType::MINUS:
  case TokenType::NOT:
    return std::make_unique<ExprOperUnaryNode>(
        std::move(parseExprOperUnaryNode(stream)));
  case TokenType::LEFT_PAREN:
    return std::make_unique<ExprGroupNode>(
        std::move(parseExprGroupNode(stream)));
  }
  throw std::runtime_error("Unexpected token in parseNudExprNode: ");
  return nullptr;
}

auto parseLedExprNode(TokenStream &stream, int32_t power,
                      std::unique_ptr<ExprNode> &&lhs)
    -> std::unique_ptr<ExprNode> {}

auto parseExprNode(TokenStream &stream, int32_t power)
    -> std::unique_ptr<ExprNode> {
  std::unique_ptr<ExprNode> result;
  auto result = std::move(parseNudExprNode(stream, power));
  while (true) {
    auto token = stream.peek();
    if (token.type == TokenType::END_OF_FILE) {
      break;
    }
    int32_t left_power = OpPowerRecoder::getInstance().getLeft(token.type);
    if (left_power <= power) {
      break;
    }
    stream.next();
    int32_t right_power = OpPowerRecoder::getInstance().getRight(token.type);
    result = parseLedExprNode(stream, right_power, std::move(result));
  }
  return result;
}

auto parseExprNode(TokenStream &stream) -> std::unique_ptr<ExprNode> {
  return parseExprNode(stream, 0);
}

auto parseExprBlockInNode(TokenStream &stream)
    -> std::unique_ptr<ExprBlockInNode> {
  switch (stream.peek().type) {
  case TokenType::LEFT_BRACE:
    return std::make_unique<ExprBlockNode>(
        std::move(parseExprBlockNode(stream)));
  case TokenType::CONST:
    return std::make_unique<ExprBlockConstNode>(
        std::move(parseExprBlockConstNode(stream)));
  case TokenType::LOOP:
    return std::make_unique<ExprLoopNode>(std::move(parseExprLoopNode(stream)));
  case TokenType::WHILE:
    return std::make_unique<ExprWhileNode>(
        std::move(parseExprWhileNode(stream)));
  case TokenType::IF:
    return std::make_unique<ExprIfNode>(std::move(parseExprIfNode(stream)));
  }
  throw std::runtime_error("Unexpected token in parseExprBlockInNode");
  return nullptr;
}

auto parseExprBlockNode(TokenStream &stream) -> std::unique_ptr<ExprBlockNode> {
  std::vector<std::unique_ptr<StmtNode>> statements;
  std::unique_ptr<ExprBlockOutNode> return_value;
  if (stream.next().type != TokenType::LEFT_BRACE) {
    throw std::runtime_error("The block expr no left brace.");
  }
  while (stream.peek().type != TokenType::RIGHT_BRACE) {
    if (stream.peek().type == TokenType::END_OF_FILE) {
      throw std::runtime_error("The block expr missed right brace.");
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
      statements.push_back(std::move(parseStmtNode(stream)));
    default:
      auto expr = std::move(parseExprNode(stream));
      if (is_instance_of<ExprBlockOutNode, ExprNode>(expr) &&
          stream.peek().type != TokenType::SEMICOLON) {
        return_value = std::move(
            dynamic_unique_ptr_cast<ExprBlockOutNode, ExprNode>(expr));
        end_flag = true;
      } else {
        statements.push_back(std::make_unique<StmtExprNode>(std::move(expr)));
      }
    }
    if (end_flag) {
      if (stream.peek().type != TokenType::RIGHT_BRACE) {
        throw std::runtime_error("The block expr missed right brace.");
      }
      break;
    }
  }
  stream.next();
  return std::make_unique<ExprBlockNode>(std::move(statements),
                                         std::move(return_value));
}

auto parseExprBlockConstNode(TokenStream &stream)
    -> std::unique_ptr<ExprBlockConstNode> {
  if (stream.next().type != TokenType::CONST) {
    throw std::runtime_error("the const block missed const.");
  }
  return std::make_unique<ExprBlockConstNode>(
      std::move(parseExprBlockNode(stream)));
}

auto parseExprLoopNode(TokenStream &stream) -> std::unique_ptr<ExprLoopNode> {
  stream.next();
  return std::make_unique<ExprLoopNode>(std::move(parseExprBlockNode(stream)));
}

auto parseExprWhileNode(TokenStream &stream) -> std::unique_ptr<ExprWhileNode> {
  stream.next();
  auto condition = std::move(parseExprNode(stream));
  if (stream.next().type != TokenType::LEFT_BRACE) {
    throw std::runtime_error("the while expr missed left brace.");
  }
  auto loop_body = std::move(parseExprBlockNode(stream));
  return std::make_unique<ExprWhileNode>(std::move(condition),
                                         std::move(loop_body));
}

auto parseExprIfNode(TokenStream &stream) -> std::unique_ptr<ExprIfNode> {
  stream.next();
  auto condition = std::move(parseExprNode(stream));
  if (stream.next().type != TokenType::LEFT_BRACE) {
    throw std::runtime_error("the if expr missed left brace.");
  }
  auto then_block = std::move(parseExprBlockNode(stream));
  std::unique_ptr<ExprBlockInNode> else_block;
  if (stream.peek().type == TokenType::ELSE) {
    stream.next();
    if (stream.next().type != TokenType::LEFT_BRACE) {
      throw std::runtime_error("the else expr missed left brace.");
    }
    else_block = std::move(parseExprBlockNode(stream));
  }
  return std::make_unique<ExprIfNode>(
      std::move(condition), std::move(then_block), std::move(else_block));
}

auto parseExprLiteralNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralNode> {
  switch (stream.peek().type) {
  case TokenType::CHARLITERAL:
    return std::make_unique<ExprLiteralCharNode>(
        std::move(parseExprLiteralCharNode(stream)));
  case TokenType::STRINGLITERAL:
  case TokenType::RAWSTRINGLITERAL:
  case TokenType::CSTRINGLITERAL:
  case TokenType::RAWCSTRINGLITERAL:
    return std::make_unique<ExprLiteralStringNode>(
        std::move(parseExprLiteralStringNode(stream)));
  case TokenType::INTEGERLITERAL:
    return std::make_unique<ExprLiteralIntNode>(
        std::move(parseExprLiteralIntNode(stream)));
  case TokenType::TRUE:
  case TokenType::FALSE:
    return std::make_unique<ExprLiteralBoolNode>(
        std::move(parseExprLiteralBoolNode(stream)));
  }
  throw std::runtime_error("Unexpected token in parseExprLiteralNode: ");
  return nullptr;
}

auto parseExprLiteralIntNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralIntNode> {
  int32_t base = 0;
  size_t pos = 0;
  std::string str = stream.next().content;
  if (str.length() > 2) {
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
      base = 16;
      pos = 2;
    } else if (str[0] == '0' && (str[1] == 'o' || str[1] == 'O')) {
      base = 8;
      pos = 2;
    } else if (str[0] == '0' && (str[1] == 'b' || str[1] == 'B')) {
      base = 2;
      pos = 2;
    }
  }
  int32_t value = std::stoi(str.substr(pos), &pos, base);
  return std::make_unique<ExprLiteralIntNode>(value);
}

auto parseExprLiteralBoolNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralBoolNode> {
  bool value = stream.next().type == TokenType::TRUE;
  return std::make_unique<ExprLiteralBoolNode>(value);
}

auto parseExprLiteralCharNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralCharNode> {
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
    case 'x':
      size_t pos = 3;
      result = std::stoi(content, &pos, 16);
      break;
    default:
      throw std::runtime_error("Unknown escape sequence.");
    }
  } else {
    result = content[1];
  }
  return std::make_unique<ExprLiteralCharNode>(static_cast<uint32_t>(result));
}

auto parseExprLiteralStringNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralStringNode> {
  std::string result;
  auto content = stream.peek().content;
  switch (stream.next().type) {
  case TokenType::STRINGLITERAL:
  case TokenType::CSTRINGLITERAL:
    result = content.substr(1, content.length() - 2);
  case TokenType::RAWSTRINGLITERAL:
  case TokenType::RAWCSTRINGLITERAL:
    int32_t hash_count = 0;
    for (int32_t i = 1; i < content.length(); ++i) {
      if (content[i] == '#') {
        ++hash_count;
      } else if (content[i] == '\"') {
        break;
      } else {
        throw std::runtime_error("The raw string literal has wrong format.");
      }
    }
    result =
        content.substr(2 + hash_count, content.length() - 3 - 2 * hash_count);
  default:
    throw std::runtime_error("The string literal has wrong format.");
  }
  return std::make_unique<ExprLiteralStringNode>(std::move(result));
}

auto parseExprOperUnaryNode(TokenStream &stream)
    -> std::unique_ptr<ExprOperUnaryNode> {
  Token token = stream.next();
  return std::make_unique<ExprOperUnaryNode>(token.type == TokenType::MINUS
                                                 ? UnaryOperator::Negate
                                                 : UnaryOperator::Not,
                                             std::move(parseExprNode(stream)));
}

auto parseExprPathNode(TokenStream &stream) -> std::unique_ptr<ExprPathNode> {}

auto parseExprGroupNode(TokenStream &stream) -> std::unique_ptr<ExprGroupNode> {
  stream.next();
  auto expr = std::move(parseExprNode(stream));
  if (stream.next().type != TokenType::RIGHT_PAREN) {
    throw std::runtime_error("The group expr missed right parenthesis.");
  }
  return std::make_unique<ExprGroupNode>(std::move(expr));
}

auto parseExprCallNode(TokenStream &stream) -> std::unique_ptr<ExprCallNode> {
  stream.next();
  auto caller = std::move(parseExprNode(stream));
  std::vector<std::unique_ptr<ExprNode>> arguments;
  if (stream.peek().type != TokenType::LEFT_PAREN) {
    throw std::runtime_error("The call expr missed left parenthesis.");
  }
  stream.next();
  while (stream.peek().type != TokenType::RIGHT_PAREN) {
    if (stream.peek().type == TokenType::END_OF_FILE) {
      throw std::runtime_error("The call expr missed right parenthesis.");
    }
    arguments.push_back(std::move(parseExprNode(stream)));
    if (stream.peek().type == TokenType::RIGHT_PAREN) {
      break;
    }
    if (stream.next().type != TokenType::COMMA) {
      throw std::runtime_error("The call expr missed comma.");
    }
  }
  stream.next();
  return std::make_unique<ExprCallNode>(std::move(caller),
                                        std::move(arguments));
}

auto parseExprBreakNode(TokenStream &stream) -> std::unique_ptr<ExprBreakNode> {
  stream.next();
  std::unique_ptr<ExprNode> value;
  if (stream.peek().type != TokenType::SEMICOLON) {
    value = std::move(parseExprNode(stream));
  } else {
    stream.next();
  }
  return std::make_unique<ExprBreakNode>(std::move(value));
}

auto parseExprReturnNode(TokenStream &stream)
    -> std::unique_ptr<ExprReturnNode> {
  stream.next();
  std::unique_ptr<ExprNode> value;
  if (stream.peek().type != TokenType::SEMICOLON) {
    value = std::move(parseExprNode(stream));
  } else {
    stream.next();
  }
  return std::make_unique<ExprReturnNode>(std::move(value));
}

auto parseExprContinueNode(TokenStream &stream)
    -> std::unique_ptr<ExprContinueNode> {
  stream.next();
  if (stream.peek().type != TokenType::SEMICOLON) {
    throw std::runtime_error("The continue expr should not have value.");
  }
  return std::make_unique<ExprContinueNode>();
}

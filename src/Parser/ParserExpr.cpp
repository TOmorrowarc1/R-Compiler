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
auto parseExprMatchNode(TokenStream &stream) -> std::unique_ptr<ExprMatchNode>;

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
auto tokenToUnaryOp(TokenType type) -> UnaryOperator;
auto parseExprMatchArm(TokenStream &stream) -> ExprMatchArm;

auto parseExprNode(TokenStream &stream) -> std::unique_ptr<ExprNode> {
  return parseExprNode(stream, 0);
}

auto parseExprNode(TokenStream &stream, int32_t power)
    -> std::unique_ptr<ExprNode> {
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
    result = parseLedExprNode(stream, token, std::move(result));
  }
  return result;
}

auto parseNudExprNode(TokenStream &stream, int32_t power)
    -> std::unique_ptr<ExprNode> {
  // It strikes me that Nud nodes are able to be LL(1) parsed.
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
    return parseExprOperUnaryNode(stream);
  case TokenType::MATCH:
    return parseExprMatchNode(stream);
  case TokenType::LEFT_PAREN: {
    stream.next();
    std::unique_ptr<ExprNode> expr;
    std::vector<std::unique_ptr<ExprNode>> elements;
    if (stream.peek().type == TokenType::RIGHT_PAREN) {
      stream.next();
      return std::make_unique<ExprTupleNode>(std::move(elements));
    }
    expr = parseExprNode(stream);
    if (stream.peek().type == TokenType::COMMA) {
      elements.push_back(std::move(expr));
      while (stream.peek().type == TokenType::COMMA) {
        stream.next();
        if (stream.peek().type == TokenType::RIGHT_PAREN) {
          break;
        }
        elements.push_back(parseExprNode(stream));
      }
      if (stream.peek().type != TokenType::RIGHT_PAREN) {
        throw std::runtime_error("Missing ) after elements.");
      }
      stream.next();
      return std::make_unique<ExprTupleNode>(std::move(elements));
    } else if (stream.peek().type == TokenType::RIGHT_PAREN) {
      stream.next();
      return std::make_unique<ExprGroupNode>(std::move(expr));
    }
    throw std::runtime_error("Unexcepted token after expr.");
    return nullptr;
  }
  case TokenType::LEFT_BRACKET:
    return parseExprArrayNode(stream);
  }
  throw std::runtime_error("Unexpected token in parseNudExprNode: ");
  return nullptr;
}

auto parseLedExprNode(TokenStream &stream, const Token &token,
                      std::unique_ptr<ExprNode> &&lhs)
    -> std::unique_ptr<ExprNode> {
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
      throw std::runtime_error("Missing ) after parameters.");
    }
    stream.next();
    return std::make_unique<ExprCallNode>(std::move(lhs),
                                          std::move(parameters));
  }
  case TokenType::LEFT_BRACKET: {
    auto index = parseExprNode(stream);
    if (stream.peek().type != TokenType::RIGHT_BRACKET) {
      throw std::runtime_error("Missing ] after an index.");
    }
    stream.next();
    return std::make_unique<ExprArrayIndexNode>(std::move(lhs),
                                                std::move(index));
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
      throw std::runtime_error("Missing } in struct fields.");
    }
    stream.next();
    auto expr_path = dynamic_unique_ptr_cast<ExprPathNode, ExprNode>(lhs);
    if (expr_path == nullptr) {
      throw std::runtime_error("The expr in struct is not a path.");
    }
    return std::make_unique<ExprStructNode>(std::move(expr_path),
                                            std::move(fields));
  }
  case TokenType::DOT: {
    auto next_token = stream.next();
    if (next_token.type == TokenType::INTEGERLITERAL) {
      return std::make_unique<ExprTupleIndexNode>(
          std::move(lhs), parseExprLiteralIntNode(stream));
    } else if (next_token.type == TokenType::IDENTIFIER) {
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
          throw std::runtime_error("Missing ) in struct fields.");
        }
        stream.next();
        return std::make_unique<ExprMethodNode>(std::move(lhs), ID,
                                                std::move(parameters));
      }
      return std::make_unique<ExprFieldNode>(std::move(lhs), ID);
    }
    throw std::runtime_error("Unexpect token type after dot.");
    return nullptr;
  }
  }
  int32_t right_power = OpPowerRecoder::getInstance().getRight(token.type);
  auto rhs = parseExprNode(stream, right_power);
  return std::make_unique<ExprOperBinaryNode>(tokenToBinaryOp(token.type),
                                              std::move(lhs), std::move(rhs));
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
  default:
    throw std::runtime_error("This token is not a binary operator.");
  }
  return BinaryOperator::ASSIGN;
}

auto tokenToUnaryOp(TokenType type) -> UnaryOperator {
  return type == TokenType::NOT ? UnaryOperator::NOT : UnaryOperator::NEGATE;
}

auto parseExprBlockInNode(TokenStream &stream)
    -> std::unique_ptr<ExprBlockInNode> {
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
  throw std::runtime_error("Unexpected token in parseExprBlockInNode");
  return nullptr;
}

auto parseStructField(TokenStream &stream) -> ExprStructField {
  ExprStructField field;
  if (stream.peek().type != TokenType::IDENTIFIER) {
    throw std::runtime_error("Fields are without identifier.");
  }
  std::string ID = stream.next().content;
  if (stream.peek().type == TokenType::COLON) {
    stream.next();
    field.expr = parseExprNode(stream);
  }
  return field;
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
  std::unique_ptr<ExprBlockNode> else_block;
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

auto parseExprMatchNode(TokenStream &stream) -> std::unique_ptr<ExprMatchNode> {
  stream.next();
  auto subject = parseExprNode(stream);
  std::vector<ExprMatchArm> arms;
  if (stream.next().type != TokenType::LEFT_BRACE) {
    throw std::runtime_error("the match expr missed left brace.");
  }
  while (stream.peek().type != TokenType::RIGHT_BRACE) {
    if (stream.peek().type == TokenType::END_OF_FILE) {
      throw std::runtime_error("the match expr missed right brace.");
    }
    arms.push_back(parseExprMatchArm(stream));
  }
  stream.next();
  return std::make_unique<ExprMatchNode>(std::move(subject), std::move(arms));
}

auto parseExprMatchArm(TokenStream &stream) -> ExprMatchArm {
  ExprMatchArm arm;
  arm.pattern = parsePatternNode(stream);
  if (stream.peek().type == TokenType::IF) {
    stream.next();
    arm.guard = std::move(parseExprNode(stream));
  }
  if (stream.peek().type != TokenType::ARROW) {
    throw std::runtime_error("The match arm missed =>.");
  }
  stream.next();
  arm.body = parseExprNode(stream);
  if (stream.peek().type == TokenType::COMMA) {
    stream.next();
  } else if (stream.peek().type != TokenType::RIGHT_BRACE &&
             is_instance_of<ExprBlockOutNode, ExprNode>(arm.body)) {
    throw std::runtime_error("The exprwithoutblock lacks comma followed.");
  }
  return arm;
}

auto parseExprLiteralNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralNode> {
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
    case 'x': {
      size_t pos = 3;
      result = std::stoi(content, &pos, 16);
      break;
    }
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
  case TokenType::RAWCSTRINGLITERAL: {
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
  }
  default:
    throw std::runtime_error("The string literal has wrong format.");
  }
  return std::make_unique<ExprLiteralStringNode>(std::move(result));
}

auto parseExprOperUnaryNode(TokenStream &stream)
    -> std::unique_ptr<ExprOperUnaryNode> {
  Token token = stream.next();
  return std::make_unique<ExprOperUnaryNode>(tokenToUnaryOp(token.type),
                                             std::move(parseExprNode(stream)));
}

auto parseExprPathNode(TokenStream &stream) -> std::unique_ptr<ExprPathNode> {
  return std::make_unique<ExprPathNode>(parsePathNode(stream));
}

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

auto parseExprArrayNode(TokenStream &stream) -> std::unique_ptr<ExprArrayNode> {
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
    throw std::runtime_error("Missing ] at the end of an array.");
  }
  stream.next();
  return std::make_unique<ExprArrayNode>(std::move(elements),
                                         std::move(length));
}
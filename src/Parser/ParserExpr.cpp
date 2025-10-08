#include "ParserTotal.hpp"
#include "Token.hpp"
#include "cast.hpp"
#include "exception.hpp"
#include "logger.hpp"
#include <string>
#include <unordered_map>

/*
The main logic of patt parser: split expr to "nud + leds" to eliminate left
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
only checked by myself.
*/

const std::vector<bindPower> nud_powers = {
    {TokenType::MINUS, 0, 23}, {TokenType::NOT, 0, 23},
    {TokenType::AND, 0, 23},   {TokenType::LOGIC_AND, 0, 23},
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
    {TokenType::LEFT_BRACKET, 24, 0},
    {TokenType::DOT, 25, 0},
    {TokenType::LEFT_BRACE, 0, 0},

    {TokenType::RIGHT_PAREN, 0, 0},
    {TokenType::RIGHT_BRACKET, 0, 0},
    {TokenType::RIGHT_BRACE, 0, 0},
    {TokenType::SEMICOLON, 0, 0},
    {TokenType::COMMA, 0, 0},

    // All possible starters for stmt/exprs after stmtexpr's semicolon.
    {TokenType::RETURN, 0, 0},
    {TokenType::BREAK, 0, 0},
    {TokenType::CONTINUE, 0, 0},
    {TokenType::INTEGERLITERAL, 0, 0},
    {TokenType::STRINGLITERAL, 0, 0},
    {TokenType::RAWSTRINGLITERAL, 0, 0},
    {TokenType::CSTRINGLITERAL, 0, 0},
    {TokenType::RAWCSTRINGLITERAL, 0, 0},
    {TokenType::CHARLITERAL, 0, 0},
    {TokenType::TRUE, 0, 0},
    {TokenType::FALSE, 0, 0},
    {TokenType::IDENTIFIER, 0, 0},
    {TokenType::IF, 0, 0},
    {TokenType::LOOP, 0, 0},
    {TokenType::WHILE, 0, 0},
    {TokenType::LET, 0, 0},
    {TokenType::FN, 0, 0},
    {TokenType::CONST, 0, 0},
    {TokenType::STRUCT, 0, 0},
    {TokenType::ENUM, 0, 0},
    {TokenType::IMPL, 0, 0},
    {TokenType::SELF, 0, 0},
    {TokenType::SELF_TYPE, 0, 0},
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
    throw std::runtime_error("Token is not a led operator.");
    return 0;
  }

  auto getRightLed(TokenType type) const -> int32_t {
    auto iter = led_power_map.find(type);
    if (iter != led_power_map.end()) {
      return iter->second.right_power;
    }
    throw std::runtime_error("Token is not a led operator.");
    return 0;
  }

  auto getRightNud(TokenType type) const -> int32_t {
    auto iter = nud_power_map.find(type);
    if (iter != nud_power_map.end()) {
      return iter->second.right_power;
    }
    throw std::runtime_error("Token is not a nud operator.");
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

auto parseExprBlockNode(TokenStream &stream) -> std::unique_ptr<ExprBlockNode>;
auto parseExprBlockConstNode(TokenStream &stream)
    -> std::unique_ptr<ExprBlockConstNode>;
auto parseExprLoopNode(TokenStream &stream) -> std::unique_ptr<ExprLoopNode>;
auto parseExprWhileNode(TokenStream &stream) -> std::unique_ptr<ExprWhileNode>;
auto parseExprIfNode(TokenStream &stream) -> std::unique_ptr<ExprIfNode>;
auto parseExprNudPathNode(TokenStream &stream) -> std::unique_ptr<ExprNode>;

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
  Logger logger = LoggerPlant::getInstance().enterFunc("parseExprNode");
  std::stringstream debug_info;
  debug_info << " left power: " << power
             << " symbol: " << stream.peek().content;
  logger.log(LogLevel::DEBUG, debug_info.str());

  auto result = parseNudExprNode(stream, power);
  while (true) {
    if (is_instance_of<ExprWhileNode, ExprNode>(result.get())) {
      // While expr can never be followed by leds.
      break;
    }
    auto token = stream.peek();
    if (token.type == TokenType::END_OF_FILE) {
      break;
    }
    int32_t left_power = OpPowerRecoder::getInstance().getLeftLed(token.type);
    if (left_power <= power) {
      break;
    }
    // Special judgement for if-no else expr.
    if (is_instance_of<ExprIfNode, ExprNode>(result.get())) {
      auto if_expr_node = static_cast<ExprIfNode *>(result.get());
      if (if_expr_node->else_block_ == nullptr) {
        break;
      }
    }
    stream.next();
    result = parseLedExprNode(stream, token, std::move(result));
  }
  return result;
}

auto parseNudExprNode(TokenStream &stream, int32_t power)
    -> std::unique_ptr<ExprNode> {
  // It strikes me that Nud nodes are able to be LL(1) parsed.
  Logger logger = LoggerPlant::getInstance().enterFunc("parseNudExprNode");
  std::stringstream debug_info;
  debug_info << " left power: " << power
             << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content;
  logger.log(LogLevel::DEBUG, debug_info.str());

  Position position = stream.peek().line;
  std::unique_ptr<ExprNode> result;
  switch (stream.peek().type) {
  case TokenType::IDENTIFIER:
  case TokenType::SELF:
  case TokenType::SELF_TYPE:
    result = parseExprNudPathNode(stream);
    break;
  case TokenType::CHARLITERAL:
    result = parseExprLiteralCharNode(stream);
    break;
  case TokenType::STRINGLITERAL:
  case TokenType::RAWSTRINGLITERAL:
  case TokenType::CSTRINGLITERAL:
  case TokenType::RAWCSTRINGLITERAL:
    result = parseExprLiteralStringNode(stream);
    break;
  case TokenType::INTEGERLITERAL:
    result = parseExprLiteralIntNode(stream);
    break;
  case TokenType::TRUE:
  case TokenType::FALSE:
    result = parseExprLiteralBoolNode(stream);
    break;
  case TokenType::BREAK:
    result = parseExprBreakNode(stream);
    break;
  case TokenType::CONTINUE:
    result = parseExprContinueNode(stream);
    break;
  case TokenType::RETURN:
    result = parseExprReturnNode(stream);
    break;
  case TokenType::MINUS:
  case TokenType::NOT:
  case TokenType::AND:
  case TokenType::LOGIC_AND:
  case TokenType::MUL:
    result = parseExprOperUnaryNode(stream);
    break;
  case TokenType::LEFT_PAREN: {
    stream.next();
    std::unique_ptr<ExprNode> expr;
    expr = parseExprNode(stream);
    if (stream.peek().type != TokenType::RIGHT_PAREN) {
      throw CompilerException("Expected ) after expression.", position);
    }
    stream.next();
    result = std::make_unique<ExprGroupNode>(std::move(expr), position);
    break;
  }
  case TokenType::LEFT_BRACE:
    result = parseExprBlockNode(stream);
    break;
  case TokenType::LEFT_BRACKET:
    result = parseExprArrayNode(stream);
    break;
  case TokenType::CONST:
    result = parseExprBlockConstNode(stream);
    break;
  case TokenType::LOOP:
    result = parseExprLoopNode(stream);
    break;
  case TokenType::WHILE:
    result = parseExprWhileNode(stream);
    break;
  case TokenType::IF:
    result = parseExprIfNode(stream);
    break;
  }
  if (result == nullptr) {
    throw CompilerException(
        "Unexpected token in expression: " + stream.peek().content, position);
  }
  return result;
}

auto parseLedExprNode(TokenStream &stream, const Token &token,
                      std::unique_ptr<ExprNode> &&lhs)
    -> std::unique_ptr<ExprNode> {
  Logger logger = LoggerPlant::getInstance().enterFunc("parseLedExprNode");
  std::stringstream debug_info;
  debug_info << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content;
  logger.log(LogLevel::DEBUG, debug_info.str());

  Position position = token.line;
  if (token.type == TokenType::LEFT_BRACKET) {
    auto index = parseExprNode(stream);
    if (stream.peek().type != TokenType::RIGHT_BRACKET) {
      throw CompilerException("Lost ] after an index.", position);
    }
    stream.next();
    return std::make_unique<ExprArrayIndexNode>(std::move(lhs),
                                                std::move(index), position);
  } else if (token.type == TokenType::DOT) {
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
  int32_t right_power = OpPowerRecoder::getInstance().getRightLed(token.type);
  auto rhs = parseExprNode(stream, right_power);
  return std::make_unique<ExprOperBinaryNode>(
      tokenToBinaryOp(token.type), std::move(lhs), std::move(rhs), position);
}

auto parseExprNudPathNode(TokenStream &stream) -> std::unique_ptr<ExprNode> {
  Logger logger = LoggerPlant::getInstance().enterFunc("parseExprNudPathNode");
  std::stringstream debug_info;
  debug_info << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content << '\n';
  logger.log(LogLevel::DEBUG, debug_info.str());

  Position position = stream.peek().line;
  auto path = parseExprPathNode(stream);
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
      throw CompilerException("Missing ) after parameters.", position);
    }
    stream.next();
    return std::make_unique<ExprCallNode>(std::move(path),
                                          std::move(parameters), position);
  } else if (stream.peek().type == TokenType::LEFT_BRACE) {
    stream.next();
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
    return std::make_unique<ExprStructNode>(std::move(path), std::move(fields),
                                            position);
  }
  return path;
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
  throw std::runtime_error("Token is not a binary operator.");
  return BinaryOperator::ASSIGN;
}

auto parseStructField(TokenStream &stream) -> ExprStructField {
  Logger logger = LoggerPlant::getInstance().enterFunc("parseStructField");
  std::stringstream debug_info;
  debug_info << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content;

  Position position = stream.peek().line;
  ExprStructField field;
  if (stream.peek().type != TokenType::IDENTIFIER) {
    throw CompilerException("Fields are without identifier.", position);
  }
  field.ID = stream.next().content;
  if (stream.peek().type == TokenType::COLON) {
    stream.next();
    field.expr = parseExprNode(stream);
  }
  debug_info << " field id: " << field.ID << '\n';
  logger.log(LogLevel::DEBUG, debug_info.str());
  return field;
}

auto parseExprBlockNode(TokenStream &stream) -> std::unique_ptr<ExprBlockNode> {
  Logger logger = LoggerPlant::getInstance().enterFunc("parseExprBlockNode");
  std::stringstream debug_info;
  debug_info << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content << '\n';

  Position position = stream.peek().line;
  std::vector<std::unique_ptr<StmtNode>> statements;
  std::unique_ptr<ExprNode> return_value;
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
      break;
    default: {
      auto expr = parseExprNode(stream);
      if (stream.peek().type == TokenType::SEMICOLON) {
        stream.next();
        auto stmt = std::make_unique<StmtExprNode>(std::move(expr), position);
        statements.push_back(std::move(stmt));
      } else if (stream.peek().type == TokenType::RIGHT_BRACE) {
        return_value = std::move(expr);
      } else {
        if (is_instance_of<ExprBlockOutNode, ExprNode>(expr)) {
          throw CompilerException(
              "The block out expr should followed by a semicolon.", position);
        }
        auto stmt = std::make_unique<StmtExprNode>(std::move(expr), position);
        statements.push_back(std::move(stmt));
      }
    }
    }
  }
  stream.next();
  logger.log(LogLevel::DEBUG, debug_info.str());
  return std::make_unique<ExprBlockNode>(std::move(statements),
                                         std::move(return_value), position);
}

auto parseExprBlockConstNode(TokenStream &stream)
    -> std::unique_ptr<ExprBlockConstNode> {
  Logger logger =
      LoggerPlant::getInstance().enterFunc("parseExprBlockConstNode");
  std::stringstream debug_info;
  debug_info << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content << '\n';

  Position position = stream.peek().line;
  if (stream.next().type != TokenType::CONST) {
    throw CompilerException("the const block missed const.", position);
  }
  logger.log(LogLevel::DEBUG, debug_info.str());
  return std::make_unique<ExprBlockConstNode>(parseExprBlockNode(stream),
                                              position);
}

auto parseExprLoopNode(TokenStream &stream) -> std::unique_ptr<ExprLoopNode> {
  Logger logger = LoggerPlant::getInstance().enterFunc("parseExprLoopNode");
  Position position = stream.peek().line;
  stream.next();
  return std::make_unique<ExprLoopNode>(parseExprBlockNode(stream), position);
}

auto parseExprWhileNode(TokenStream &stream) -> std::unique_ptr<ExprWhileNode> {
  Logger logger = LoggerPlant::getInstance().enterFunc("parseExprWhileNode");
  Position position = stream.peek().line;
  stream.next();
  auto condition = parseCondition(stream);
  auto loop_body = parseExprBlockNode(stream);
  return std::make_unique<ExprWhileNode>(std::move(condition),
                                         std::move(loop_body), position);
}

auto parseExprIfNode(TokenStream &stream) -> std::unique_ptr<ExprIfNode> {
  Logger logger = LoggerPlant::getInstance().enterFunc("parseExprIfNode");
  std::stringstream debug_info;
  debug_info << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content;

  Position position = stream.peek().line;
  stream.next();
  auto condition = parseCondition(stream);
  auto then_block = parseExprBlockNode(stream);
  std::unique_ptr<ExprNode> else_block;
  if (stream.peek().type == TokenType::ELSE) {
    stream.next();
    if (stream.peek().type == TokenType::IF) {
      else_block = parseExprIfNode(stream);
    } else {
      else_block = parseExprBlockNode(stream);
    }
  }
  logger.log(LogLevel::DEBUG, debug_info.str());
  return std::make_unique<ExprIfNode>(std::move(condition),
                                      std::move(then_block),
                                      std::move(else_block), position);
}

auto parseCondition(TokenStream &stream) -> std::unique_ptr<ExprNode> {
  Logger logger = LoggerPlant::getInstance().enterFunc("parseCondition");
  std::stringstream debug_info;
  debug_info << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content;

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
  logger.log(LogLevel::DEBUG, debug_info.str());
  return condition;
}

auto parseExprLiteralNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralNode> {
  Logger logger = LoggerPlant::getInstance().enterFunc("parseExprLiteralNode");
  std::stringstream debug_info;
  debug_info << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content;

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
  logger.log(LogLevel::DEBUG, debug_info.str());
  return nullptr;
}

auto parseExprLiteralIntNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralIntNode> {
  Logger logger =
      LoggerPlant::getInstance().enterFunc("parseExprLiteralIntNode");
  std::stringstream debug_info;
  debug_info << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content;

  Position position = stream.peek().line;
  std::string str = stream.next().content;
  ExprLiteralIntNode::IntType int_type = ExprLiteralIntNode::IntType::NUM;
  int32_t base = 10;
  size_t begin_pos = 0;
  size_t end_pos = str.length();
  if (str[0] == '0') {
    if (std::isalpha(str[1])) {
      str[1] = std::tolower(str[1]);
    }
    switch (str[1]) {
    case 'x':
      base = 16;
      begin_pos = 2;
      break;
    case 'o':
      base = 8;
      begin_pos = 2;
      break;
    case 'b':
      base = 2;
      begin_pos = 2;
      break;
    }
  }
  for (size_t i = begin_pos; i < end_pos; ++i) {
    char c = str[i];
    if (std::isalpha(c)) {
      str[i] = std::tolower(c);
      if (base == 16) {
        if (str[i] < 'a' || str[i] > 'f') {
          end_pos = i;
          switch (str[i]) {
          case 'i':
            int_type = str[i + 1] == '3' ? ExprLiteralIntNode::IntType::I32
                                         : ExprLiteralIntNode::IntType::ISIZE;
            break;
          case 'u':
            int_type = str[i + 1] == '3' ? ExprLiteralIntNode::IntType::U32
                                         : ExprLiteralIntNode::IntType::USIZE;
            break;
          default:
            throw CompilerException("Unknown suffix in int literal.", position);
          }
        }
      } else {
        end_pos = i;
        switch (str[i]) {
        case 'i':
          int_type = str[i + 1] == '3' ? ExprLiteralIntNode::IntType::I32
                                       : ExprLiteralIntNode::IntType::ISIZE;
          break;
        case 'u':
          int_type = str[i + 1] == '3' ? ExprLiteralIntNode::IntType::U32
                                       : ExprLiteralIntNode::IntType::USIZE;
          break;
        default:
          throw CompilerException("Unknown suffix in int literal.", position);
        }
      }
    }
  }
  std::string_view numberic_part =
      std::string_view(str).substr(begin_pos, end_pos);
  std::string cleaned_literal;
  for (char c : numberic_part) {
    if (c != '_') {
      cleaned_literal += c;
    }
  }
  if (cleaned_literal.empty()) {
    throw CompilerException("Literal contains no digits.", position);
  }
  uint32_t value =
      std::stoul(cleaned_literal.substr(begin_pos), &begin_pos, base);
  logger.log(LogLevel::DEBUG, debug_info.str());
  return std::make_unique<ExprLiteralIntNode>(value, int_type, position);
}

auto parseExprLiteralBoolNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralBoolNode> {
  Logger logger =
      LoggerPlant::getInstance().enterFunc("parseExprLiteralBoolNode");
  std::stringstream debug_info;
  debug_info << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content;

  Position position = stream.peek().line;
  bool value = stream.next().type == TokenType::TRUE;
  logger.log(LogLevel::DEBUG, debug_info.str());
  return std::make_unique<ExprLiteralBoolNode>(value, position);
}

auto parseExprLiteralCharNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralCharNode> {
  Logger logger =
      LoggerPlant::getInstance().enterFunc("parseExprLiteralCharNode");
  std::stringstream debug_info;
  debug_info << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content;

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
  logger.log(LogLevel::DEBUG, debug_info.str());
  return std::make_unique<ExprLiteralCharNode>(static_cast<uint32_t>(result),
                                               position);
}

auto parseExprLiteralStringNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralStringNode> {
  Logger logger =
      LoggerPlant::getInstance().enterFunc("parseExprLiteralStringNode");
  std::stringstream debug_info;
  debug_info << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content;

  Position position = stream.peek().line;
  std::string result;
  auto content = stream.peek().content;
  switch (stream.next().type) {
  case TokenType::STRINGLITERAL:
  case TokenType::CSTRINGLITERAL:
    result = content.substr(1, content.length() - 2);
    break;
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
    break;
  }
  default:
    throw CompilerException("The string literal has wrong format.", position);
  }
  logger.log(LogLevel::DEBUG, debug_info.str());
  return std::make_unique<ExprLiteralStringNode>(std::move(result), position);
}

auto parseExprOperUnaryNode(TokenStream &stream)
    -> std::unique_ptr<ExprOperUnaryNode> {
  Logger logger =
      LoggerPlant::getInstance().enterFunc("parseExprOperUnaryNode");
  std::stringstream debug_info;
  debug_info << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content;

  Position position = stream.peek().line;
  UnaryOperator op;
  Token token = stream.next();
  switch (token.type) {
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
  case TokenType::LOGIC_AND: {
    op = UnaryOperator::REF;
    if (stream.peek().type == TokenType::MUT) {
      stream.next();
      op = UnaryOperator::MUTREF;
    }
    auto expr = parseExprNode(
        stream, OpPowerRecoder::getInstance().getRightNud(token.type));
    auto ref =
        std::make_unique<ExprOperUnaryNode>(op, std::move(expr), position);
    op = UnaryOperator::REF;
    return std::make_unique<ExprOperUnaryNode>(op, std::move(ref), position);
  }
  default:
    throw CompilerException("Unexpected token in unary operator.", position);
  }
  auto expr = parseExprNode(
      stream, OpPowerRecoder::getInstance().getRightNud(token.type));
  logger.log(LogLevel::DEBUG, debug_info.str());
  return std::make_unique<ExprOperUnaryNode>(op, std::move(expr), position);
}

auto parseExprPathNode(TokenStream &stream) -> std::unique_ptr<ExprPathNode> {
  Logger logger = LoggerPlant::getInstance().enterFunc("parseExprPathNode");
  std::stringstream debug_info;
  debug_info << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content;

  Position position = stream.peek().line;
  logger.log(LogLevel::DEBUG, debug_info.str());
  return std::make_unique<ExprPathNode>(parsePathNode(stream), position);
}

auto parseExprGroupNode(TokenStream &stream) -> std::unique_ptr<ExprGroupNode> {
  Logger logger = LoggerPlant::getInstance().enterFunc("parseExprGroupNode");
  std::stringstream debug_info;
  debug_info << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content;

  Position position = stream.peek().line;
  stream.next();
  auto expr = std::move(parseExprNode(stream));
  if (stream.next().type != TokenType::RIGHT_PAREN) {
    throw CompilerException("The group expr missed right parent.", position);
  }
  logger.log(LogLevel::DEBUG, debug_info.str());
  return std::make_unique<ExprGroupNode>(std::move(expr), position);
}

auto parseExprCallNode(TokenStream &stream) -> std::unique_ptr<ExprCallNode> {
  Logger logger = LoggerPlant::getInstance().enterFunc("parseExprCallNode");
  std::stringstream debug_info;
  debug_info << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content;

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
  logger.log(LogLevel::DEBUG, debug_info.str());
  return std::make_unique<ExprCallNode>(std::move(caller), std::move(arguments),
                                        position);
}

auto parseExprBreakNode(TokenStream &stream) -> std::unique_ptr<ExprBreakNode> {
  Logger logger = LoggerPlant::getInstance().enterFunc("parseExprBreakNode");
  std::stringstream debug_info;
  debug_info << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content;

  Position position = stream.peek().line;
  stream.next();
  std::unique_ptr<ExprNode> value;
  if (stream.peek().type != TokenType::SEMICOLON) {
    value = parseExprNode(stream);
  } else {
    stream.next();
  }
  logger.log(LogLevel::DEBUG, debug_info.str());
  return std::make_unique<ExprBreakNode>(std::move(value), position);
}

auto parseExprReturnNode(TokenStream &stream)
    -> std::unique_ptr<ExprReturnNode> {
  Logger logger = LoggerPlant::getInstance().enterFunc("parseExprReturnNode");
  std::stringstream debug_info;
  debug_info << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content;

  Position position = stream.peek().line;
  stream.next();
  std::unique_ptr<ExprNode> value;
  if (stream.peek().type != TokenType::SEMICOLON) {
    value = parseExprNode(stream);
  } else {
    stream.next();
  }
  logger.log(LogLevel::DEBUG, debug_info.str());
  return std::make_unique<ExprReturnNode>(std::move(value), position);
}

auto parseExprContinueNode(TokenStream &stream)
    -> std::unique_ptr<ExprContinueNode> {
  Logger logger = LoggerPlant::getInstance().enterFunc("parseExprContinueNode");
  std::stringstream debug_info;
  debug_info << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content;

  Position position = stream.peek().line;
  stream.next();
  if (stream.peek().type != TokenType::SEMICOLON) {
    throw CompilerException("Continue expr should not have value.", position);
  }
  logger.log(LogLevel::DEBUG, debug_info.str());
  return std::make_unique<ExprContinueNode>(position);
}

auto parseExprArrayNode(TokenStream &stream) -> std::unique_ptr<ExprArrayNode> {
  Logger logger = LoggerPlant::getInstance().enterFunc("parseExprArrayNode");
  std::stringstream debug_info;
  debug_info << " on line: " << stream.peek().line.getLine()
             << " symbol: " << stream.peek().content;

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
  logger.log(LogLevel::DEBUG, debug_info.str());
  return std::make_unique<ExprArrayNode>(std::move(elements), std::move(length),
                                         position);
}
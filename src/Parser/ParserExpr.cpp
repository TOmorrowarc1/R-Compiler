#include "ParserTotal.hpp"
#include "token.hpp"
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

auto parseExprBlockInNode(TokenStream &stream)
    -> std::unique_ptr<ExprBlockInNode>;
auto parseExprBlockOutNode(TokenStream &stream)
    -> std::unique_ptr<ExprBlockOutNode>;
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
auto parseExprLiteralByteNode(TokenStream &stream)
    -> std::unique_ptr<ExprLiteralByteNode>;
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
  case TokenType::IF:
    return std::make_unique<ExprIfNode>(std::move(parseExprIfNode(stream)));
  case TokenType::WHILE:
    return std::make_unique<ExprWhileNode>(
        std::move(parseExprWhileNode(stream)));
  case TokenType::LOOP:
    return std::make_unique<ExprLoopNode>(std::move(parseExprLoopNode(stream)));
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
    return std::make_unique<ExprOperUnaryNode>(
        std::move(parseExprOperUnaryNode(stream)));
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

auto parseExprNode(TokenStream &stream, int32_t power)
    -> std::unique_ptr<ExprNode> {
  auto lhs = std::move(parseNudExprNode(stream, power));
  while (true) {
    auto token = stream.peek();
    if (token.type == TokenType::END_OF_FILE) {
      break;
    }
    int32_t left_power = OpPowerRecoder::getInstance().getLeft(token.type);
    if (left_power < power) {
      break;
    }
    stream.next();
    int32_t right_power = OpPowerRecoder::getInstance().getRight(token.type);
    auto rhs = std::move(parseExprNode(stream, right_power));
    lhs = std::make_unique<ExprOperBinaryNode>(std::move(lhs), token,
                                               std::move(rhs));
  }
}

auto parseExprNode(TokenStream &stream) -> std::unique_ptr<ExprNode> {
  return parseExprNode(stream, 0);
}

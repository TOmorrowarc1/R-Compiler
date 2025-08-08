#pragma once
#include "ExprOperatorNode.hpp"

enum class BinaryOperator {
  ASSIGN,
  AS_CAST,

  EQUAL,
  NOT_EQUAL,
  LESS_THAN,
  LESS_EQUAL,
  GREATER_THAN,
  GREATER_EQUAL,

  LOGIC_AND,
  LOGIC_OR,
  NOT,

  PLUS,
  MINUS,
  MUL,
  DIV,
  MOD_CAL,
  XOR,
  AND,
  OR,
  LEFT_SHIFT,
  RIGHT_SHIFT,

  PLUS_EQUAL,
  MINUS_EQUAL,
  MUL_EQUAL,
  DIV_EQUAL,
  MOD_EQUAL,
  XOR_EQUAL,
  AND_EQUAL,
  OR_EQUAL,
  LEFT_SHIFT_EQUAL,
  RIGHT_SHIFT_EQUAL,
};

class ExprOperBinaryNode : public ExprOperatorNode {
private:
  BinaryOperator op_;
  std::unique_ptr<ExprNode> lhs_;
  std::unique_ptr<ExprNode> rhs_;

public:
  ExprOperBinaryNode(BinaryOperator op, std::unique_ptr<ExprNode> &&lhs,
                     std::unique_ptr<ExprNode> &&rhs);
  ~ExprOperBinaryNode() = default;
  void accept(Visitor &visitor) override;
};
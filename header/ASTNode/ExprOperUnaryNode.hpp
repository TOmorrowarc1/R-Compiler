#pragma once
#include "ExprOperatorNode.hpp"

enum class UnaryOperator { Negate, Not, BitwiseNot };
class ExprOperUnaryNode : public ExprOperatorNode {
private:
  UnaryOperator op_;
  std::unique_ptr<ExprNode> operand_;

public:
  ExprOperUnaryNode(UnaryOperator op, std::unique_ptr<ExprNode> &&operand);
  ~ExprOperUnaryNode() = default;
  void accept(Visitor &visitor) override;
};
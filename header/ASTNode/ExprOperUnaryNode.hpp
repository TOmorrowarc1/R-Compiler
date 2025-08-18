#pragma once
#include "ExprOperatorNode.hpp"

enum class UnaryOperator { NEGATE, NOT };
class ExprOperUnaryNode : public ExprOperatorNode {
public:
  UnaryOperator op_;
  std::unique_ptr<ExprNode> operand_;

  ExprOperUnaryNode(UnaryOperator op, std::unique_ptr<ExprNode> &&operand);
  ~ExprOperUnaryNode();
  void accept(Visitor &visitor) override;
};
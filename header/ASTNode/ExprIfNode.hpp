#pragma once
#include "ExprANode.hpp"

class ExprBlockNode;

class ExprIfNode : public ExprBlockInNode {
public:
  std::unique_ptr<ExprNode> condition_;
  std::unique_ptr<ExprBlockNode> then_block_;
  std::unique_ptr<ExprNode> else_block_;

  ExprIfNode(std::unique_ptr<ExprNode> &&condition,
             std::unique_ptr<ExprBlockNode> &&then_block,
             std::unique_ptr<ExprNode> &&else_block, Position position);
  ~ExprIfNode();
  void accept(Visitor &visitor) override;
};
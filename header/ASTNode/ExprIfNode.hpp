#pragma once
#include "ExprANode.hpp"

class ExprBlockNode;

class ExprIfNode : public ExprBlockInNode {
private:
  std::unique_ptr<ExprNode> condition_;
  std::unique_ptr<ExprBlockNode> then_block_;
  std::unique_ptr<ExprBlockNode> else_block_;
public:
  ExprIfNode(std::unique_ptr<ExprNode> &&condition,
             std::unique_ptr<ExprBlockNode> &&then_block,
             std::unique_ptr<ExprBlockNode> &&else_block);
  ~ExprIfNode();
  void accept(Visitor &visitor) override;
};
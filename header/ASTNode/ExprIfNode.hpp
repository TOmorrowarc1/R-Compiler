#pragma once
#include "ExprANode.hpp"

class ExprIfNode : public ExprBlockInNode {
private:
  std::unique_ptr<ExprNode> condition_;
  std::unique_ptr<ExprBlockInNode> then_block_;
  std::unique_ptr<ExprBlockInNode> else_block_;
public:
  ExprIfNode(std::unique_ptr<ExprNode> &&condition,
             std::unique_ptr<ExprBlockInNode> &&then_block,
             std::unique_ptr<ExprBlockInNode> &&else_block);
  ~ExprIfNode() = default;
  void accept(Visitor &visitor) override;
};
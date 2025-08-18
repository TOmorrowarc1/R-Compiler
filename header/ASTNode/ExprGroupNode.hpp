#pragma once
#include "ExprANode.hpp"

class ExprGroupNode : public ExprBlockOutNode {
public:
  std::unique_ptr<ExprNode> expr_;

  ExprGroupNode(std::unique_ptr<ExprNode> &&expr);
  ~ExprGroupNode();
  void accept(Visitor &visitor) override;
};
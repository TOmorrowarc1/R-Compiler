#pragma once
#include "ExprANode.hpp"

class ExprGroupNode : public ExprBlockOutNode {
private:
  std::unique_ptr<ExprNode> expr_;
public:
  ExprGroupNode(std::unique_ptr<ExprNode> &&expr);
  ~ExprGroupNode();
  void accept(Visitor &visitor) override;
};
#pragma once
#include "StmtANode.hpp"

class ExprNode;

class StmtExprNode : public StmtNode {
public:
  std::unique_ptr<ExprNode> expr_;

  StmtExprNode(std::unique_ptr<ExprNode> expr);
  ~StmtExprNode();
  void accept(Visitor &visitor) override;
};
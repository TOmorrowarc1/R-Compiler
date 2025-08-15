#pragma once
#include "StmtANode.hpp"

class ExprNode;

class StmtExprNode : public StmtNode {
private:
  std::unique_ptr<ExprNode> expr_;
public:
  StmtExprNode(std::unique_ptr<ExprNode> expr);
  ~StmtExprNode();
  void accept(Visitor &visitor) override;
};
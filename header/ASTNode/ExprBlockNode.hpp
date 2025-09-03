#pragma once
#include "ExprANode.hpp"

class StmtNode;

class ExprBlockNode : public ExprBlockInNode {
public:
  std::vector<std::unique_ptr<StmtNode>> statements_;
  std::unique_ptr<ExprNode> return_value_;

  ExprBlockNode(std::vector<std::unique_ptr<StmtNode>> &&statements,
                std::unique_ptr<ExprNode> &&return_value, Position position);
  ~ExprBlockNode();
  void accept(Visitor &visitor) override;
};

class ExprBlockConstNode : public ExprBlockInNode {
public:
  std::unique_ptr<ExprBlockNode> block_expr_;

  ExprBlockConstNode(std::unique_ptr<ExprBlockNode> &&block_expr,
                     Position position);
  ~ExprBlockConstNode();
  void accept(Visitor &visitor) override;
};
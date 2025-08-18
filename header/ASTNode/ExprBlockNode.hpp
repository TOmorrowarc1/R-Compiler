#pragma once
#include "ExprANode.hpp"

class StmtNode;

class ExprBlockNode : public ExprBlockInNode {
public:
  std::vector<std::unique_ptr<StmtNode>> statements_;
  std::unique_ptr<ExprBlockOutNode> return_value_;

  ExprBlockNode(std::vector<std::unique_ptr<StmtNode>> &&statements,
                std::unique_ptr<ExprBlockOutNode> &&return_value);
  ~ExprBlockNode();
  void accept(Visitor &visitor) override;
};

class ExprBlockConstNode : public ExprBlockInNode {
private:
  std::unique_ptr<ExprBlockNode> block_expr_;

public:
  ExprBlockConstNode(std::unique_ptr<ExprBlockNode> &&block_expr);
  ~ExprBlockConstNode();
  void accept(Visitor &visitor) override;
};
#pragma once
#include "ExprANode.hpp"

class StmtNode;
class ExprBlockNode : public ExprBlockInNode {
private:
  std::vector<std::unique_ptr<StmtNode>> statements_;
  std::unique_ptr<ExprBlockOutNode> return_value_;

public:
  ExprBlockNode(std::vector<std::unique_ptr<StmtNode>> &&statements,
                std::unique_ptr<ExprBlockOutNode> &&return_value)
      : statements_(std::move(statements)),
        return_value_(std::move(return_value)) {}

  ~ExprBlockNode() = default;

  void accept(Visitor &visitor) override;
};

class ExprBlockConstNode : public ExprBlockInNode {
private:
  std::unique_ptr<ExprBlockNode> block_expr_;

public:
  ExprBlockConstNode(std::unique_ptr<ExprBlockNode> &&block_expr);
  ~ExprBlockConstNode() = default;
  void accept(Visitor &visitor) override;
};
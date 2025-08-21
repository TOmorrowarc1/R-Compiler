#pragma once
#include "ExprANode.hpp"

class ExprBreakNode : public ExprBlockOutNode {
public:
  std::unique_ptr<ExprNode> value_;

  ExprBreakNode(std::unique_ptr<ExprNode> &&value, Position position);
  ~ExprBreakNode();
  void accept(Visitor &visitor) override;
};

class ExprContinueNode : public ExprBlockOutNode {
public:
  ExprContinueNode(Position position);
  ~ExprContinueNode();
  void accept(Visitor &visitor) override;
};

class ExprReturnNode : public ExprBlockOutNode {
public:
  std::unique_ptr<ExprNode> value_;

  ExprReturnNode(std::unique_ptr<ExprNode> &&value, Position position);
  ~ExprReturnNode();
  void accept(Visitor &visitor) override;
};
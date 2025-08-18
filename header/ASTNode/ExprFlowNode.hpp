#pragma once
#include "ExprANode.hpp"

class ExprBreakNode : public ExprBlockOutNode {
public:
  std::unique_ptr<ExprNode> value_;

  ExprBreakNode(std::unique_ptr<ExprNode> &&value);
  ~ExprBreakNode();
  void accept(Visitor &visitor) override;
};

class ExprContinueNode : public ExprBlockOutNode {
public:
  ExprContinueNode();
  ~ExprContinueNode();
  void accept(Visitor &visitor) override;
};

class ExprReturnNode : public ExprBlockOutNode {
public:
  std::unique_ptr<ExprNode> value_;

  ExprReturnNode(std::unique_ptr<ExprNode> &&value);
  ~ExprReturnNode();
  void accept(Visitor &visitor) override;
};
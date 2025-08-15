#pragma once
#include "ExprANode.hpp"

class ExprBreakNode : public ExprBlockOutNode {
private:
  std::unique_ptr<ExprNode> value_;

public:
  ExprBreakNode(std::unique_ptr<ExprNode> &&value);
  ~ExprBreakNode();
  void accept(Visitor &visitor) override;
};

class ExprContinueNode : public ExprBlockOutNode {
private:
public:
  ExprContinueNode();
  ~ExprContinueNode();
  void accept(Visitor &visitor) override;
};

class ExprReturnNode : public ExprBlockOutNode {
private:
  std::unique_ptr<ExprNode> value_;

public:
  ExprReturnNode(std::unique_ptr<ExprNode> &&value);
  ~ExprReturnNode();
  void accept(Visitor &visitor) override;
};
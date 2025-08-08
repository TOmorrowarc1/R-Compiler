#pragma once
#include "ExprANode.hpp"

class ExprBreakNode : public ExprBlockOutNode {
private:
  std::unique_ptr<ExprNode> value_;

public:
  ExprBreakNode(std::unique_ptr<ExprNode> &&value);
  ~ExprBreakNode() = default;
  void accept(Visitor &visitor) override;
};

class ExprContinueNode : public ExprBlockOutNode {
private:
public:
  ExprContinueNode();
  ~ExprContinueNode() = default;
  void accept(Visitor &visitor) override;
};

class ExprReturnNode : public ExprBlockOutNode {
private:
  std::unique_ptr<ExprNode> value_;

public:
  ExprReturnNode(std::unique_ptr<ExprNode> &&value);
  ~ExprReturnNode() = default;
  void accept(Visitor &visitor) override;
};
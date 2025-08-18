#pragma once
#include "ASTNode.hpp"

class ExprNode : public ASTNode {
public:
  ExprNode() : ASTNode() {}
  virtual ~ExprNode() = default;
  virtual void accept(Visitor &visitor) override = 0;
};

class ExprBlockInNode : public ExprNode {
public:
  ExprBlockInNode() : ExprNode() {}
  virtual ~ExprBlockInNode() = default;
  virtual void accept(Visitor &visitor) override = 0;
};

class ExprBlockOutNode : public ExprNode {
public:
  ExprBlockOutNode() : ExprNode() {}
  virtual ~ExprBlockOutNode() = default;
  virtual void accept(Visitor &visitor) override = 0;
};
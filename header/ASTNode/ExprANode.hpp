#pragma once
#include "ASTNode.hpp"

class ExprNode : public ASTNode {
private:
public:
  ExprNode() : ASTNode() {}
  virtual ~ExprNode() = default;
  virtual void accept(Visitor &visitor) override = 0;
};

class ExprBlockInNode : public ExprNode {
private:
public:
  ExprBlockInNode() : ExprNode() {}
  virtual ~ExprBlockInNode() = default;
  virtual void accept(Visitor &visitor) override;
};

class ExprBlockOutNode : public ExprNode {
private:
public:
  ExprBlockOutNode() : ExprNode() {}
  virtual ~ExprBlockOutNode() = default;
  virtual void accept(Visitor &visitor) override;
};
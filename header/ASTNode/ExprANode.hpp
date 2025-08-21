#pragma once
#include "ASTNode.hpp"

class ValueInfo;

class ExprNode : public ASTNode {
public:
  std::unique_ptr<ValueInfo> value_info_;

  ExprNode(Position position) : ASTNode(position), value_info_(nullptr) {}
  virtual ~ExprNode() = default;
  virtual void accept(Visitor &visitor) override = 0;
};

class ExprBlockInNode : public ExprNode {
public:
  ExprBlockInNode(Position position) : ExprNode(position) {}
  virtual ~ExprBlockInNode() = default;
  virtual void accept(Visitor &visitor) override = 0;
};

class ExprBlockOutNode : public ExprNode {
public:
  ExprBlockOutNode(Position position) : ExprNode(position) {}
  virtual ~ExprBlockOutNode() = default;
  virtual void accept(Visitor &visitor) override = 0;
};
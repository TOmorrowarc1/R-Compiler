#pragma once
#include "ExprANode.hpp"

class ExprLiteralNode : public ExprBlockOutNode {
public:
  ExprLiteralNode(Position position) : ExprBlockOutNode(position){};
  virtual ~ExprLiteralNode() = default;
  virtual void accept(Visitor &visitor) override = 0;
};

class ExprLiteralIntNode : public ExprLiteralNode {
public:
  enum class IntType { I32, U32, NUM };
  IntType int_type_;
  int32_t value_;

  ExprLiteralIntNode(int32_t value, IntType int_type, Position position);
  ~ExprLiteralIntNode();
  void accept(Visitor &visitor) override;
};

class ExprLiteralBoolNode : public ExprLiteralNode {
private:
  bool value_;

public:
  ExprLiteralBoolNode(bool value, Position position);
  ~ExprLiteralBoolNode();
  void accept(Visitor &visitor) override;
};

class ExprLiteralCharNode : public ExprLiteralNode {
private:
  uint32_t value_;

public:
  ExprLiteralCharNode(uint32_t value, Position position);
  ~ExprLiteralCharNode();
  void accept(Visitor &visitor) override;
};

class ExprLiteralStringNode : public ExprLiteralNode {
private:
  std::string value_;

public:
  ExprLiteralStringNode(const std::string &value, Position position);
  ~ExprLiteralStringNode();
  void accept(Visitor &visitor) override;
};
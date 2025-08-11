#pragma once
#include "ExprANode.hpp"

class ExprLiteralNode : public ExprBlockOutNode {
private:
public:
  ExprLiteralNode() : ExprBlockOutNode(){};
  ~ExprLiteralNode() = default;
  virtual void accept(Visitor &visitor) override = 0;
};

class ExprLiteralIntNode : public ExprLiteralNode {
private:
  int32_t value_;

public:
  ExprLiteralIntNode(int32_t value);
  ~ExprLiteralIntNode() = default;
  void accept(Visitor &visitor) override;
};

class ExprLiteralBoolNode : public ExprLiteralNode {
private:
  bool value_;

public:
  ExprLiteralBoolNode(bool value);
  ~ExprLiteralBoolNode() = default;
  void accept(Visitor &visitor) override;
};

class ExprLiteralCharNode : public ExprLiteralNode {
private:
  uint32_t value_;

public:
  ExprLiteralCharNode(uint32_t value);
  ~ExprLiteralCharNode() = default;
  void accept(Visitor &visitor) override;
};

class ExprLiteralStringNode : public ExprLiteralNode {
private:
  std::string value_;

public:
  ExprLiteralStringNode(const std::string &value);
  ~ExprLiteralStringNode() = default;
  void accept(Visitor &visitor) override;
};
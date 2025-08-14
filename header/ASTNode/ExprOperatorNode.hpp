#pragma once
#include "ExprANode.hpp"

class ExprOperatorNode : public ExprBlockOutNode {
private:
public:
  ExprOperatorNode() : ExprBlockOutNode(){};
  ~ExprOperatorNode() = default;
  virtual void accept(Visitor &visitor) override = 0;
};
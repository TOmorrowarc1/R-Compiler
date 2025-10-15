#pragma once
#include "ExprANode.hpp"

class ExprOperatorNode : public ExprBlockOutNode {
public:
  ExprOperatorNode(Position position) : ExprBlockOutNode(position){};
  virtual ~ExprOperatorNode() = default;
  virtual void accept(Visitor &visitor) override = 0;
};
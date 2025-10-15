#pragma once
#include "ASTNode.hpp"

class PatternNode : public ASTNode {
public:
  PatternNode(Position position) : ASTNode(position){};
  virtual ~PatternNode() = default;
  virtual void accept(Visitor &visitor) override = 0;
};
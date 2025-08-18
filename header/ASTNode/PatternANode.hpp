#pragma once
#include "ASTNode.hpp"

class PatternNode : public ASTNode {
public:
  PatternNode() : ASTNode(){};
  virtual ~PatternNode() = default;
  virtual void accept(Visitor &visitor) override = 0;
};
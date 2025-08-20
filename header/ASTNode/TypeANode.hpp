#pragma once
#include "ASTNode.hpp"

class TypeNode : public ASTNode {
public:
  TypeNode() : ASTNode(){};
  virtual ~TypeNode() = default;
  virtual void accept(Visitor &visitor) = 0;
};
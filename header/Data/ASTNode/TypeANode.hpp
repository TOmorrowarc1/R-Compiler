#pragma once
#include "ASTNode.hpp"

class TypeNode : public ASTNode {
public:
  TypeNode(Position position) : ASTNode(position){};
  virtual ~TypeNode() = default;
  virtual void accept(Visitor &visitor) = 0;
};
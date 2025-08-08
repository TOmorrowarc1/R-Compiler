#pragma once
#include "ASTNode.hpp"

class TypeNode : public ASTNode {
private:
public:
  TypeNode() : ASTNode(){};
  virtual ~TypeNode() = default;
  virtual void accept(Visitor &visitor) = 0;
};
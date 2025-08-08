#pragma once
#include "ASTNode.hpp"

class ItemNode : public ASTNode {
private:
public:
  ItemNode() : ASTNode() {}
  virtual void accept(Visitor &visitor) override = 0;
};
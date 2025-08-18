#pragma once
#include "ASTNode.hpp"

class ItemNode : public ASTNode {
public:
  ItemNode() : ASTNode() {}
  virtual ~ItemNode() = default;
  virtual void accept(Visitor &visitor) override = 0;
};
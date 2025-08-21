#pragma once
#include "ASTNode.hpp"

class ItemNode : public ASTNode {
public:
  ItemNode(Position position) : ASTNode(position) {}
  virtual ~ItemNode() = default;
  virtual void accept(Visitor &visitor) override = 0;
};
#pragma once
#include "TypeANode.hpp"

class TypeUnitNode : public TypeNode {
public:
  TypeUnitNode(Position position);
  ~TypeUnitNode() override;
  void accept(Visitor &visitor) override;
};
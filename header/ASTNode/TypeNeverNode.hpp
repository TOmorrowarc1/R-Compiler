#pragma once
#include "TypeANode.hpp"

class TypeNeverNode : public TypeNode {
private:
public:
  TypeNeverNode();
  ~TypeNeverNode() = default;
  void accept(Visitor &visitor) override;
};
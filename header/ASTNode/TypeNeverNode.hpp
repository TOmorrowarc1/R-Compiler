#pragma once
#include "TypeANode.hpp"

class TypeNeverNode : public TypeNode {
private:
public:
  TypeNeverNode();
  ~TypeNeverNode();
  void accept(Visitor &visitor) override;
};
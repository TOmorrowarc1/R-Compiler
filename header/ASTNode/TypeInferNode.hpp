#pragma once
#include "TypeANode.hpp"

class TypeInferNode : public TypeNode {
public:
  TypeInferNode();
  ~TypeInferNode();
  void accept(Visitor &visitor) override;
};
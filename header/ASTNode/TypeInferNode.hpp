#pragma once
#include "TypeANode.hpp"

class TypeInferNode : public TypeNode {
private:
public:
  TypeInferNode();
  ~TypeInferNode();
  void accept(Visitor &visitor) override;
};
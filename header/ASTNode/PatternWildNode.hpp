#pragma once
#include "PatternANode.hpp"

class PatternWildNode : public PatternNode {
public:
  PatternWildNode();
  ~PatternWildNode();
  void accept(Visitor &visitor) override;
};
#pragma once
#include "PatternANode.hpp"

class PatternWildNode : public PatternNode {
public:
  PatternWildNode(Position position);
  ~PatternWildNode();
  void accept(Visitor &visitor) override;
};
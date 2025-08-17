#pragma once
#include "PatternANode.hpp"

class PatternWildNode : public PatternNode {
private:
public:
  PatternWildNode();
  ~PatternWildNode();
  void accept(Visitor &visitor) override;
};
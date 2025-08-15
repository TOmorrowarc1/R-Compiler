#pragma once
#include "PatternANode.hpp"

class PatternWildNode : public PatternOneNode {
private:
public:
  PatternWildNode();
  ~PatternWildNode();
  void accept(Visitor &visitor) override;
};
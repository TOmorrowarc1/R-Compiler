#pragma once
#include "PatternANode.hpp"

class PatternWildNode : public PatternOneNode {
private:
public:
  PatternWildNode();
  ~PatternWildNode() = default;
  void accept(Visitor &visitor) override;
};
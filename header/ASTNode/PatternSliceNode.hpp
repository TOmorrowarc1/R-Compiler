#pragma once
#include "PatternANode.hpp"

class PatternSliceNode : public PatternOneNode {
private:
  std::vector<std::unique_ptr<PatternNode>> patterns_;

public:
  PatternSliceNode(std::vector<std::unique_ptr<PatternNode>> &&pattern);
  ~PatternSliceNode();
  void accept(Visitor &visitor) override;
};
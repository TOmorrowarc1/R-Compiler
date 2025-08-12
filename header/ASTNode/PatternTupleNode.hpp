#pragma once
#include "PatternANode.hpp"

class PatternTupleNode : public PatternOneNode {
private:
  std::vector<std::unique_ptr<PatternNode>> patterns_;

public:
  PatternTupleNode(std::vector<std::unique_ptr<PatternNode>> &&pattern);
  ~PatternTupleNode() = default;
  void accept(Visitor &visitor) override;
};
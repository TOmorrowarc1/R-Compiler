#pragma once
#include "PatternANode.hpp"

class PatternTupleNode : public PatternNode {
private:
  std::vector<std::unique_ptr<PatternNode>> patterns_;

public:
  PatternTupleNode(std::vector<std::unique_ptr<PatternNode>> &&pattern);
  ~PatternTupleNode();
  void accept(Visitor &visitor) override;
};
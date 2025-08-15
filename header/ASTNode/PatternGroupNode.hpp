#pragma once
#include "PatternANode.hpp"

class PatternGroupNode : public PatternOneNode {
private:
  std::unique_ptr<PatternNode> pattern_;

public:
  PatternGroupNode(std::unique_ptr<PatternNode> &&pattern);
  ~PatternGroupNode();
  void accept(Visitor &visitor) override;
};
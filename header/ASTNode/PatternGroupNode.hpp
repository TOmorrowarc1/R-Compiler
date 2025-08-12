#pragma once
#include "PatternANode.hpp"

class PatternGroupNode : public PatternOneNode {
private:
  std::unique_ptr<PatternNode> expr_;

public:
  PatternGroupNode(std::unique_ptr<PatternNode> &&expr);
  ~PatternGroupNode() = default;
  void accept(Visitor &visitor) override;
};
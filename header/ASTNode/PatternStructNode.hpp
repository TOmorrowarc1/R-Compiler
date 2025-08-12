#pragma once
#include "PatternANode.hpp"

struct PatternStructField {
  std::string identifier_;
  std::unique_ptr<PatternNode> pattern_;
};

class PatternStructNode : public PatternOneNode {
private:
  std::vector<std::unique_ptr<PatternStructField>> patterns_;

public:
  PatternStructNode(std::vector<std::unique_ptr<PatternNode>> &&pattern);
  ~PatternStructNode() = default;
  void accept(Visitor &visitor) override;
};
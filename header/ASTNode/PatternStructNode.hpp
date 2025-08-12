#pragma once
#include "PatternANode.hpp"

struct PatternStructField {
  std::string identifier_;
  std::unique_ptr<PatternNode> pattern_;
};

class PatternStructNode : public PatternOneNode {
private:
  std::vector<PatternStructField> patterns_;
public:
  PatternStructNode(std::vector<PatternStructField> &&pattern);
  ~PatternStructNode() = default;
  void accept(Visitor &visitor) override;
};
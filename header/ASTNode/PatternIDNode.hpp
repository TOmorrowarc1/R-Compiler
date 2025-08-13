#pragma once
#include "PatternANode.hpp"

class PatternIDNode : public PatternOneNode {
private:
  std::unique_ptr<PatternNode> pattern_;
  std::string identifier_;

public:
  PatternIDNode(std::unique_ptr<PatternNode> &&pattern,
                const std::string &identifier);
  ~PatternIDNode() = default;
  void accept(Visitor &visitor) override;
};
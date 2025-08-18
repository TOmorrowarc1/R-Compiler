#pragma once
#include "PatternANode.hpp"

class PatternIDNode : public PatternNode {
public:
  std::string identifier_;
  std::unique_ptr<PatternNode> pattern_;

  PatternIDNode(const std::string &identifier,
                std::unique_ptr<PatternNode> &&pattern);
  ~PatternIDNode();
  void accept(Visitor &visitor) override;
};
#pragma once
#include "PatternANode.hpp"

class PatternIDNode : public PatternNode {
private:
  std::string identifier_;
  std::unique_ptr<PatternNode> pattern_;

public:
  PatternIDNode(const std::string &identifier,
                std::unique_ptr<PatternNode> &&pattern);
  ~PatternIDNode();
  void accept(Visitor &visitor) override;
};
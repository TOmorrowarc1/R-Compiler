#pragma once
#include "PatternANode.hpp"

class PatternIDNode : public PatternOneNode {
private:
  std::string identifier_;
  std::unique_ptr<PatternOneNode> pattern_;

public:
  PatternIDNode(const std::string &identifier,
                std::unique_ptr<PatternOneNode> &&pattern);
  ~PatternIDNode();
  void accept(Visitor &visitor) override;
};
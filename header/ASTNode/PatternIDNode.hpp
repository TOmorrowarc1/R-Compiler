#pragma once
#include "PatternANode.hpp"

class PatternIDNode : public PatternOneNode {
private:
  std::string identifier_;

public:
  PatternIDNode(const std::string &identifier_);
  ~PatternIDNode() = default;
  void accept(Visitor &visitor) override;
};
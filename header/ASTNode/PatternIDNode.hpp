#pragma once
#include "PatternANode.hpp"

class PatternIDNode : public PatternNode {
public:
  enum class IDType { PLAIN, MUT };
  std::string identifier_;
  std::unique_ptr<PatternNode> pattern_;
  IDType id_type_;

  PatternIDNode(const std::string &identifier,
                std::unique_ptr<PatternNode> &&pattern, IDType id_type,
                Position position);
  ~PatternIDNode();
  void accept(Visitor &visitor) override;
};
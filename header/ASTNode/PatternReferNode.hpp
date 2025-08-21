#include "PatternANode.hpp"

class PatternReferNode : public PatternNode {
public:
  std::unique_ptr<PatternNode> pattern_;
  bool is_mutable_;

  PatternReferNode(std::unique_ptr<PatternNode> &&pattern, bool is_mutable,
                   Position position);
  ~PatternReferNode() override;
  void accept(Visitor &visitor) override;
};
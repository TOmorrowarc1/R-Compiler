#pragma once
#include "ExprANode.hpp"

class ExprUnderScoreNode : public ExprBlockOutNode {
public:
  ExprUnderScoreNode(Position position);
  ~ExprUnderScoreNode() override;
  void accept(Visitor &visitor) override;
};
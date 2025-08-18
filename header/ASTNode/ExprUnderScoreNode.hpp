#pragma once
#include "ExprANode.hpp"

class ExprUnderScoreNode : public ExprBlockOutNode {
public:
  ExprUnderScoreNode();
  ~ExprUnderScoreNode() override;
  void accept(Visitor &visitor) override;
};
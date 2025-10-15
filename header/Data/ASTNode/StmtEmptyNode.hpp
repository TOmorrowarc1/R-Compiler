#pragma once
#include "StmtANode.hpp"

class StmtEmptyNode : public StmtNode {
public:
  StmtEmptyNode(Position position);
  ~StmtEmptyNode();
  void accept(Visitor &visitor) override;
};
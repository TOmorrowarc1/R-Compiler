#pragma once
#include "StmtANode.hpp"

class StmtEmptyNode : public StmtNode {
public:
  StmtEmptyNode();
  ~StmtEmptyNode();
  void accept(Visitor &visitor) override;
};
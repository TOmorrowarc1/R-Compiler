#pragma once
#include "StmtANode.hpp"

class StmtEmptyNode : public StmtNode {
public:
  StmtEmptyNode() : StmtNode() {}
  ~StmtEmptyNode() = default;
  void accept(Visitor &visitor) override;
};
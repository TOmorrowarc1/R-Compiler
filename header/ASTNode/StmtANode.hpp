#pragma once
#include "ASTNode.hpp"

class StmtNode : public ASTNode {
public:
  StmtNode() : ASTNode() {}
  virtual ~StmtNode() = default;
  virtual void accept(Visitor &visitor) = 0;
};
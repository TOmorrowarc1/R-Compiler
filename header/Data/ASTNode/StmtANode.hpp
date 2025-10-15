#pragma once
#include "ASTNode.hpp"

class StmtNode : public ASTNode {
public:
  StmtNode(Position position) : ASTNode(position) {}
  virtual ~StmtNode() = default;
  virtual void accept(Visitor &visitor) = 0;
};
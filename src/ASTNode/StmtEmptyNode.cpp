#include "StmtEmptyNode.hpp"
#include "Visitor.hpp"

StmtEmptyNode::StmtEmptyNode() {}
StmtEmptyNode::~StmtEmptyNode() = default;
void StmtEmptyNode::accept(Visitor &visitor) { visitor.visit(this); }
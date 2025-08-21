#include "StmtEmptyNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

StmtEmptyNode::StmtEmptyNode(Position position) : StmtNode(position) {}
StmtEmptyNode::~StmtEmptyNode() = default;
void StmtEmptyNode::accept(Visitor &visitor) { visitor.visit(this); }
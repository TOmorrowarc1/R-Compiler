#include "StmtItemNode.hpp"
#include "Visitor.hpp"

StmtItemNode::StmtItemNode(std::unique_ptr<ItemNode> item)
    : StmtNode(), item_(std::move(item)) {}

void StmtItemNode::accept(Visitor &visitor) { visitor.visit(*this); }
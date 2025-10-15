#include "StmtItemNode.hpp"
#include "ItemANode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

StmtItemNode::StmtItemNode(std::unique_ptr<ItemNode> &&item, Position position)
    : StmtNode(position), item_(std::move(item)) {}
StmtItemNode::~StmtItemNode() = default;
void StmtItemNode::accept(Visitor &visitor) { visitor.visit(this); }
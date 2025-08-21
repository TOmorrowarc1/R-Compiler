#include "ExprFieldNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

ExprFieldNode::ExprFieldNode(std::unique_ptr<ExprNode> &&instance,
                             const std::string &ID, Position position)
    : ExprBlockOutNode(position), instance_(std::move(instance)), ID_(ID) {}
ExprFieldNode::~ExprFieldNode() = default;
void ExprFieldNode::accept(Visitor &visitor) { visitor.visit(this); }
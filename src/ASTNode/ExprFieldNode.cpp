#include "ExprFieldNode.hpp"
#include "Visitor.hpp"

ExprFieldNode::ExprFieldNode(std::unique_ptr<ExprNode> &&instance,
                             const std::string &ID)
    : ExprBlockOutNode(), instance_(std::move(instance)), ID_(ID) {}

void ExprFieldNode::accept(Visitor &visitor) { visitor.visit(*this); }
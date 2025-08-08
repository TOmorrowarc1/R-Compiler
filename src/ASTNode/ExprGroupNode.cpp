#include "ExprGroupNode.hpp"

ExprGroupNode::ExprGroupNode(std::unique_ptr<ExprNode> &&expr)
    : ExprBlockOutNode(), expr_(std::move(expr)) {}

void ExprGroupNode::accept(Visitor &visitor) { visitor.visit(*this); }
#include "ExprGroupNode.hpp"
#include "Visitor.hpp"

ExprGroupNode::ExprGroupNode(std::unique_ptr<ExprNode> &&expr)
    : ExprBlockOutNode(), expr_(std::move(expr)) {}
ExprGroupNode::~ExprGroupNode() = default;
void ExprGroupNode::accept(Visitor &visitor) { visitor.visit(*this); }
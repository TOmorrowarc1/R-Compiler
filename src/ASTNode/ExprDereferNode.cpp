#include "ExprDereferNode.hpp"
#include "Visitor.hpp"

ExprDereferNode::ExprDereferNode(std::unique_ptr<ExprNode> &&expr,
                                 Position position)
    : ExprBlockOutNode(position), expr_(std::move(expr)) {}
ExprDereferNode::~ExprDereferNode() = default;
void ExprDereferNode::accept(Visitor &visitor) { visitor.visit(this); }
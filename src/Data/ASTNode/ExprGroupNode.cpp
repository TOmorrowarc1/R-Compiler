#include "ExprGroupNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

ExprGroupNode::ExprGroupNode(std::unique_ptr<ExprNode> &&expr,
                             Position position)
    : ExprBlockOutNode(position), expr_(std::move(expr)) {}
ExprGroupNode::~ExprGroupNode() = default;
void ExprGroupNode::accept(Visitor &visitor) { visitor.visit(this); }
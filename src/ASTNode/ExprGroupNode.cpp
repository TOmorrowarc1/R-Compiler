#include "ExprGroupNode.hpp"
#include "Visitor.hpp"
#include "ValueInfo.hpp"

ExprGroupNode::ExprGroupNode(std::unique_ptr<ExprNode> &&expr)
    : ExprBlockOutNode(), expr_(std::move(expr)) {}
ExprGroupNode::~ExprGroupNode() = default;
void ExprGroupNode::accept(Visitor &visitor) { visitor.visit(this); }
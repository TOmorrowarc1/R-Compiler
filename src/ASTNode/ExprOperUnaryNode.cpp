#include "ExprOperUnaryNode.hpp"
#include "Visitor.hpp"

ExprOperUnaryNode::ExprOperUnaryNode(UnaryOperator op,
                                     std::unique_ptr<ExprNode> &&operand)
    : ExprOperatorNode(), op_(op), operand_(std::move(operand)) {}
ExprOperUnaryNode::~ExprOperUnaryNode() = default;
void ExprOperUnaryNode::accept(Visitor &visitor) { visitor.visit(*this); }
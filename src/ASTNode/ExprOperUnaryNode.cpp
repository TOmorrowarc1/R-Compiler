#include "ExprOperUnaryNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

ExprOperUnaryNode::ExprOperUnaryNode(UnaryOperator op,
                                     std::unique_ptr<ExprNode> &&operand,
                                     Position position)
    : ExprOperatorNode(position), op_(op), operand_(std::move(operand)) {}
ExprOperUnaryNode::~ExprOperUnaryNode() = default;
void ExprOperUnaryNode::accept(Visitor &visitor) { visitor.visit(this); }
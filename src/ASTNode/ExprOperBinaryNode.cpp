#include "ExprOperBinaryNode.hpp"
#include "Visitor.hpp"
#include "ValueInfo.hpp"

ExprOperBinaryNode::ExprOperBinaryNode(BinaryOperator op,
                                       std::unique_ptr<ExprNode> &&lhs,
                                       std::unique_ptr<ExprNode> &&rhs)
    : ExprOperatorNode(), op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}
ExprOperBinaryNode::~ExprOperBinaryNode() = default;
void ExprOperBinaryNode::accept(Visitor &visitor) { visitor.visit(this); };
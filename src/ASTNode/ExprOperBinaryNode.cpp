#include "ExprOperBinaryNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

ExprOperBinaryNode::ExprOperBinaryNode(BinaryOperator op,
                                       std::unique_ptr<ExprNode> &&lhs,
                                       std::unique_ptr<ExprNode> &&rhs,
                                       Position position)
    : ExprOperatorNode(position), op_(op), lhs_(std::move(lhs)),
      rhs_(std::move(rhs)) {}
ExprOperBinaryNode::~ExprOperBinaryNode() = default;
void ExprOperBinaryNode::accept(Visitor &visitor) { visitor.visit(this); };
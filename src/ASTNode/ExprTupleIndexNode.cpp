#include "ExprTupleIndexNode.hpp"
#include "Visitor.hpp"

ExprTupleIndexNode::ExprTupleIndexNode(
    std::unique_ptr<ExprNode> &&tuple,
    std::unique_ptr<ExprLiteralIntNode> &&index)
    : ExprBlockOutNode(), tuple_(std::move(tuple)), index_(std::move(index)) {}

void ExprTupleIndexNode::accept(Visitor &visitor) { visitor.visit(*this); }
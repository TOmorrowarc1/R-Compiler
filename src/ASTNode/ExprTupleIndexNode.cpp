#include "ExprTupleIndexNode.hpp"
#include "ExprLiteralNode.hpp"
#include "Visitor.hpp"

ExprTupleIndexNode::ExprTupleIndexNode(
    std::unique_ptr<ExprNode> &&tuple,
    std::unique_ptr<ExprLiteralIntNode> &&index)
    : ExprBlockOutNode(), tuple_(std::move(tuple)), index_(std::move(index)) {}
ExprTupleIndexNode::~ExprTupleIndexNode() = default;
void ExprTupleIndexNode::accept(Visitor &visitor) { visitor.visit(this); }
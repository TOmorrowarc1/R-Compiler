#include "ExprTupleNode.hpp"
#include "Visitor.hpp"

ExprTupleNode::ExprTupleNode(std::vector<std::unique_ptr<ExprNode>> &&elements)
    : ExprBlockOutNode(), elements_(std::move(elements)) {}
ExprTupleNode::~ExprTupleNode() = default;
void ExprTupleNode::accept(Visitor &visitor) { visitor.visit(this); }
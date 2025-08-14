#include "ExprStructNode.hpp"
#include "Visitor.hpp"

ExprStructNode::ExprStructNode(std::unique_ptr<ExprPathNode> &&path,
                               std::vector<ExprStructField> &&fields)
    : ExprBlockOutNode(), path_(std::move(path)), fields_(std::move(fields)) {}

void ExprStructNode::accept(Visitor &visitor) { visitor.visit(*this); }
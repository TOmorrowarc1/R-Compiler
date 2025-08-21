#include "ExprStructNode.hpp"
#include "ExprPathNode.hpp"
#include "Visitor.hpp"
#include "ValueInfo.hpp"

// Allow possible path not simple for the pratt needs lhs to be a expr.
ExprStructNode::ExprStructNode(std::unique_ptr<ExprPathNode> &&path,
                               std::vector<ExprStructField> &&fields)
    : ExprBlockOutNode(), path_(std::move(path)), fields_(std::move(fields)) {}
ExprStructNode::~ExprStructNode() = default;
void ExprStructNode::accept(Visitor &visitor) { visitor.visit(this); }
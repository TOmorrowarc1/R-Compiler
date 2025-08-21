#include "ExprBorrowNode.hpp"
#include "Visitor.hpp"

ExprBorrowNode::ExprBorrowNode(std::unique_ptr<ExprNode> &&expr,
                               bool is_mutable, Position position)
    : ExprBlockOutNode(position), expr_(std::move(expr)),
      is_mutable_(is_mutable) {}

ExprBorrowNode::~ExprBorrowNode() = default;

void ExprBorrowNode::accept(Visitor &visitor) { visitor.visit(this); }
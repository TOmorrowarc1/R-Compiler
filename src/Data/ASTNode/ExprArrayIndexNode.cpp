#include "ExprArrayIndexNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

ExprArrayIndexNode::ExprArrayIndexNode(std::unique_ptr<ExprNode> &&array,
                                       std::unique_ptr<ExprNode> &&index,
                                       Position position)
    : ExprBlockOutNode(position), array_(std::move(array)),
      index_(std::move(index)) {}
ExprArrayIndexNode::~ExprArrayIndexNode() = default;
void ExprArrayIndexNode::accept(Visitor &visitor) { visitor.visit(this); }
#include "ExprArrayIndexNode.hpp"
#include "Visitor.hpp"
#include "ValueInfo.hpp"

ExprArrayIndexNode::ExprArrayIndexNode(std::unique_ptr<ExprNode> &&array,
                                       std::unique_ptr<ExprNode> &&index)
    : ExprBlockOutNode(), array_(std::move(array)), index_(std::move(index)) {}
ExprArrayIndexNode::~ExprArrayIndexNode() = default;
void ExprArrayIndexNode::accept(Visitor &visitor) { visitor.visit(this); }
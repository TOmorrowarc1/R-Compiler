#include "ExprIfNode.hpp"
#include "Visitor.hpp"

ExprIfNode::ExprIfNode(std::unique_ptr<ExprNode> &&condition,
                       std::unique_ptr<ExprBlockInNode> &&then_block,
                       std::unique_ptr<ExprBlockInNode> &&else_block)
    : ExprBlockInNode(), condition_(std::move(condition)),
      then_block_(std::move(then_block)), else_block_(std::move(else_block)) {}

void ExprIfNode::accept(Visitor &visitor) { visitor.visit(*this); }
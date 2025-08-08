#include "ExprLoopNode.hpp"
#include "Visitor.hpp"

ExprLoopNode::ExprLoopNode(std::unique_ptr<ExprBlockInNode> &&loop_body)
    : ExprBlockInNode(), loop_body_(std::move(loop_body)) {}

void ExprLoopNode::accept(Visitor &visitor) { visitor.visit(*this); }

ExprWhileNode::ExprWhileNode(std::unique_ptr<ExprNode> &&condition,
                             std::unique_ptr<ExprBlockInNode> &&loop_body)
    : ExprBlockInNode(), condition_(std::move(condition)),
      loop_body_(std::move(loop_body)) {}

void ExprWhileNode::accept(Visitor &visitor) { visitor.visit(*this); }
#include "ExprLoopNode.hpp"
#include "ExprBlockNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

ExprLoopNode::ExprLoopNode(std::unique_ptr<ExprBlockNode> &&loop_body,
                           Position position)
    : ExprBlockInNode(position), loop_body_(std::move(loop_body)) {}
ExprLoopNode::~ExprLoopNode() = default;
void ExprLoopNode::accept(Visitor &visitor) { visitor.visit(this); }

ExprWhileNode::ExprWhileNode(std::unique_ptr<ExprNode> &&condition,
                             std::unique_ptr<ExprBlockNode> &&loop_body,
                             Position position)
    : ExprBlockInNode(position), condition_(std::move(condition)),
      loop_body_(std::move(loop_body)) {}
ExprWhileNode::~ExprWhileNode() = default;
void ExprWhileNode::accept(Visitor &visitor) { visitor.visit(this); }
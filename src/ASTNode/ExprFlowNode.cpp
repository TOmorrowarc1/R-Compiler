#include "ExprFlowNode.hpp"
#include "Visitor.hpp"

ExprBreakNode::ExprBreakNode(std::unique_ptr<ExprNode> &&value)
    : ExprBlockOutNode(), value_(std::move(value)) {}
ExprBreakNode::~ExprBreakNode() = default;
void ExprBreakNode::accept(Visitor &visitor) { visitor.visit(this); }

ExprContinueNode::ExprContinueNode() : ExprBlockOutNode() {}
ExprContinueNode::~ExprContinueNode() = default;
void ExprContinueNode::accept(Visitor &visitor) { visitor.visit(this); }

ExprReturnNode::ExprReturnNode(std::unique_ptr<ExprNode> &&value)
    : ExprBlockOutNode(), value_(std::move(value)) {}
ExprReturnNode::~ExprReturnNode() = default;
void ExprReturnNode::accept(Visitor &visitor) { visitor.visit(this); }
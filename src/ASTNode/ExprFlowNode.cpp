#include "ExprFlowNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

ExprBreakNode::ExprBreakNode(std::unique_ptr<ExprNode> &&value,
                             Position position)
    : ExprBlockOutNode(position), value_(std::move(value)) {}
ExprBreakNode::~ExprBreakNode() = default;
void ExprBreakNode::accept(Visitor &visitor) { visitor.visit(this); }

ExprContinueNode::ExprContinueNode(Position position)
    : ExprBlockOutNode(position) {}
ExprContinueNode::~ExprContinueNode() = default;
void ExprContinueNode::accept(Visitor &visitor) { visitor.visit(this); }

ExprReturnNode::ExprReturnNode(std::unique_ptr<ExprNode> &&value,
                               Position position)
    : ExprBlockOutNode(position), value_(std::move(value)) {}
ExprReturnNode::~ExprReturnNode() = default;
void ExprReturnNode::accept(Visitor &visitor) { visitor.visit(this); }
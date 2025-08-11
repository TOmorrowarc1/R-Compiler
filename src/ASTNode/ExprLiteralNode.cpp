#include "ExprLiteralNode.hpp"
#include "Visitor.hpp"

ExprLiteralIntNode::ExprLiteralIntNode(int32_t value)
    : ExprLiteralNode(), value_(value) {}

void ExprLiteralIntNode::accept(Visitor &visitor) { visitor.visit(*this); }

ExprLiteralBoolNode::ExprLiteralBoolNode(bool value)
    : ExprLiteralNode(), value_(value) {}

void ExprLiteralBoolNode::accept(Visitor &visitor) { visitor.visit(*this); }

ExprLiteralCharNode::ExprLiteralCharNode(uint32_t value)
    : ExprLiteralNode(), value_(value) {}

void ExprLiteralCharNode::accept(Visitor &visitor) { visitor.visit(*this); }

ExprLiteralStringNode::ExprLiteralStringNode(const std::string &value)
    : ExprLiteralNode(), value_(value) {}

void ExprLiteralStringNode::accept(Visitor &visitor) { visitor.visit(*this); }
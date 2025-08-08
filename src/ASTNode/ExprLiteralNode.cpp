#include"ExprLiteralNode.hpp"
#include "Visitor.hpp"

ExprLiteralIntNode::ExprLiteralIntNode(int64_t value)
    : ExprLiteralNode(), value_(static_cast<int32_t>(value)) {}

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

ExprLiteralByteNode::ExprLiteralByteNode(uint8_t value)
    : ExprLiteralNode(), value_(value) {}
    
void ExprLiteralByteNode::accept(Visitor &visitor) { visitor.visit(*this); }
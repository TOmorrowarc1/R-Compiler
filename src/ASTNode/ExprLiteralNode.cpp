#include "ExprLiteralNode.hpp"
#include "Visitor.hpp"

ExprLiteralIntNode::ExprLiteralIntNode(int32_t value)
    : ExprLiteralNode(), value_(value) {}
ExprLiteralIntNode::~ExprLiteralIntNode() = default;
void ExprLiteralIntNode::accept(Visitor &visitor) { visitor.visit(*this); }

ExprLiteralBoolNode::ExprLiteralBoolNode(bool value)
    : ExprLiteralNode(), value_(value) {}
ExprLiteralBoolNode::~ExprLiteralBoolNode() = default;
void ExprLiteralBoolNode::accept(Visitor &visitor) { visitor.visit(*this); }

ExprLiteralCharNode::ExprLiteralCharNode(uint32_t value)
    : ExprLiteralNode(), value_(value) {}
ExprLiteralCharNode::~ExprLiteralCharNode() = default;
void ExprLiteralCharNode::accept(Visitor &visitor) { visitor.visit(*this); }

ExprLiteralStringNode::ExprLiteralStringNode(const std::string &value)
    : ExprLiteralNode(), value_(value) {}
ExprLiteralStringNode::~ExprLiteralStringNode() = default;
void ExprLiteralStringNode::accept(Visitor &visitor) { visitor.visit(*this); }
#include "ExprLiteralNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

ExprLiteralIntNode::ExprLiteralIntNode(int32_t value, bool sign,
                                       Position position)
    : ExprLiteralNode(position), value_(value), signed_(sign) {}
ExprLiteralIntNode::~ExprLiteralIntNode() = default;
void ExprLiteralIntNode::accept(Visitor &visitor) { visitor.visit(this); }

ExprLiteralBoolNode::ExprLiteralBoolNode(bool value, Position position)
    : ExprLiteralNode(position), value_(value) {}
ExprLiteralBoolNode::~ExprLiteralBoolNode() = default;
void ExprLiteralBoolNode::accept(Visitor &visitor) { visitor.visit(this); }

ExprLiteralCharNode::ExprLiteralCharNode(uint32_t value, Position position)
    : ExprLiteralNode(position), value_(value) {}
ExprLiteralCharNode::~ExprLiteralCharNode() = default;
void ExprLiteralCharNode::accept(Visitor &visitor) { visitor.visit(this); }

ExprLiteralStringNode::ExprLiteralStringNode(const std::string &value,
                                             Position position)
    : ExprLiteralNode(position), value_(value) {}
ExprLiteralStringNode::~ExprLiteralStringNode() = default;
void ExprLiteralStringNode::accept(Visitor &visitor) { visitor.visit(this); }
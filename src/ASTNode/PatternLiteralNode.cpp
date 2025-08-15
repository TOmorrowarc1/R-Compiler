#include "PatternLiteralNode.hpp"
#include "ExprLiteralNode.hpp"
#include "Visitor.hpp"

PatternLiteralNode::PatternLiteralNode(std::unique_ptr<ExprLiteralNode> &&expr,
                                       bool minus)
    : PatternOneNode(), expr_(std::move(expr)), minus_(minus) {}
PatternLiteralNode::~PatternLiteralNode() = default;
void PatternLiteralNode::accept(Visitor &visitor) { visitor.visit(*this); }
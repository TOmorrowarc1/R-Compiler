#include "PatternLiteralNode.hpp"
#include "ExprLiteralNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

PatternLiteralNode::PatternLiteralNode(std::unique_ptr<ExprLiteralNode> &&expr,
                                       bool minus, Position position)
    : PatternNode(position), expr_(std::move(expr)), minus_(minus) {}
PatternLiteralNode::~PatternLiteralNode() = default;
void PatternLiteralNode::accept(Visitor &visitor) { visitor.visit(this); }
#include "ExprMatchNode.hpp"
#include "PatternANode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

ExprMatchNode::ExprMatchNode(std::unique_ptr<ExprNode> &&subject,
                             std::vector<ExprMatchArm> &&arms,
                             Position position)
    : ExprBlockInNode(position), subject_(std::move(subject)),
      arms_(std::move(arms)) {}

ExprMatchNode::~ExprMatchNode() = default;

void ExprMatchNode::accept(Visitor &visitor) { visitor.visit(this); }
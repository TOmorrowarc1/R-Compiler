#include "ExprIfNode.hpp"
#include "ExprBlockNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

ExprIfNode::ExprIfNode(std::unique_ptr<ExprNode> &&condition,
                       std::unique_ptr<ExprBlockNode> &&then_block,
                       std::unique_ptr<ExprNode> &&else_block,
                       Position position)
    : ExprBlockInNode(position), condition_(std::move(condition)),
      then_block_(std::move(then_block)), else_block_(std::move(else_block)) {}

ExprIfNode::~ExprIfNode() = default;

void ExprIfNode::accept(Visitor &visitor) { visitor.visit(this); }
#include "ExprBlockNode.hpp"
#include "StmtANode.hpp"
#include "Visitor.hpp"

ExprBlockNode::ExprBlockNode(
    std::vector<std::unique_ptr<StmtNode>> &&statements,
    std::unique_ptr<ExprBlockOutNode> &&return_value)
    : ExprBlockInNode(), statements_(std::move(statements)),
      return_value_(std::move(return_value)) {}
ExprBlockNode::~ExprBlockNode() = default;
void ExprBlockNode::accept(Visitor &visitor) { visitor.visit(*this); }

ExprBlockConstNode::ExprBlockConstNode(
    std::unique_ptr<ExprBlockNode> &&block_expr)
    : ExprBlockInNode(), block_expr_(std::move(block_expr)) {}
ExprBlockConstNode::~ExprBlockConstNode() = default;
void ExprBlockConstNode::accept(Visitor &visitor) { visitor.visit(*this); }

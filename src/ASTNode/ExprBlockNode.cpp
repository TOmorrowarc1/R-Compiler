#include "ExprBlockNode.hpp"
#include "StmtANode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

ExprBlockNode::ExprBlockNode(
    std::vector<std::unique_ptr<StmtNode>> &&statements,
    std::unique_ptr<ExprBlockOutNode> &&return_value, Position position)
    : ExprBlockInNode(position), statements_(std::move(statements)),
      return_value_(std::move(return_value)) {}
ExprBlockNode::~ExprBlockNode() = default;
void ExprBlockNode::accept(Visitor &visitor) { visitor.visit(this); }

ExprBlockConstNode::ExprBlockConstNode(
    std::unique_ptr<ExprBlockNode> &&block_expr, Position position)
    : ExprBlockInNode(position), block_expr_(std::move(block_expr)) {}
ExprBlockConstNode::~ExprBlockConstNode() = default;
void ExprBlockConstNode::accept(Visitor &visitor) { visitor.visit(this); }

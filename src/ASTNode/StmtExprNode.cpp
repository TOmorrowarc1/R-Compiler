#include "StmtExprNode.hpp"
#include "ExprANode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

StmtExprNode::StmtExprNode(std::unique_ptr<ExprNode> &&expr, Position position)
    : StmtNode(position), expr_(std::move(expr)) {}
StmtExprNode::~StmtExprNode() = default;
void StmtExprNode::accept(Visitor &visitor) { visitor.visit(this); }
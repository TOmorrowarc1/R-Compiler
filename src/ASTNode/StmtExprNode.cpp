#include "StmtExprNode.hpp"
#include "ExprANode.hpp"
#include "Visitor.hpp"

StmtExprNode::StmtExprNode(std::unique_ptr<ExprNode> expr)
    : StmtNode(), expr_(std::move(expr)) {}
StmtExprNode::~StmtExprNode() = default;
void StmtExprNode::accept(Visitor &visitor) { visitor.visit(this); }
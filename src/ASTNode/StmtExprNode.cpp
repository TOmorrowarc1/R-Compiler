#include "StmtExprNode.hpp"
#include "Visitor.hpp"

StmtExprNode::StmtExprNode(std::unique_ptr<ExprNode> expr)
    : StmtNode(), expr_(std::move(expr)) {}
    
void StmtExprNode::accept(Visitor &visitor) { visitor.visit(*this); }
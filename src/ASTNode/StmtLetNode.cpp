#include "StmtLetNode.hpp"
#include "Visitor.hpp"

StmtLetNode::StmtLetNode(std::unique_ptr<PatternOneNode> &&pattern,
                         std::unique_ptr<TypeNode> &&type,
                         std::unique_ptr<ExprNode> &&init_value,
                         std::unique_ptr<ExprBlockNode> &&else_body)
    : StmtNode(), pattern_(std::move(pattern)), type_(std::move(type)),
      init_value_(std::move(init_value)), else_body_(std::move(else_body)) {}

void StmtLetNode::accept(Visitor &visitor) { visitor.visit(*this); }
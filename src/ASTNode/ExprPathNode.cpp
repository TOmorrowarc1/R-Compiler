#include "ExprPathNode.hpp"
#include "Visitor.hpp"

ExprPathNode::ExprPathNode(const std::string &path) : ExprNode(), path_(path) {}

void ExprPathNode::accept(Visitor &visitor) { visitor.visit(*this); }
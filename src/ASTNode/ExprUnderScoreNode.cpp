#include "ExprUnderScoreNode.hpp"
#include "Visitor.hpp"

ExprUnderScoreNode::ExprUnderScoreNode() : ExprBlockOutNode() {}
ExprUnderScoreNode::~ExprUnderScoreNode() = default;
void ExprUnderScoreNode::accept(Visitor &visitor) { visitor.visit(this); }
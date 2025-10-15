#include "ExprUnderScoreNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

ExprUnderScoreNode::ExprUnderScoreNode(Position position)
    : ExprBlockOutNode(position) {}
ExprUnderScoreNode::~ExprUnderScoreNode() = default;
void ExprUnderScoreNode::accept(Visitor &visitor) { visitor.visit(this); }
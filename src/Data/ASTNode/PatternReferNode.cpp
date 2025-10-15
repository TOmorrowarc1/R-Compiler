#include "PatternReferNode.hpp"
#include "Visitor.hpp"

PatternReferNode::PatternReferNode(std::unique_ptr<PatternNode> &&pattern,
                                   bool is_mutable, Position position)
    : PatternNode(position), is_mutable_(is_mutable),
      pattern_(std::move(pattern)) {}
PatternReferNode::~PatternReferNode() = default;
void PatternReferNode::accept(Visitor &visitor) { visitor.visit(this); }
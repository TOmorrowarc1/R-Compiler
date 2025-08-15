#include "PatternIDNode.hpp"
#include "Visitor.hpp"

PatternIDNode::PatternIDNode(const std::string &identifier,
                             std::unique_ptr<PatternOneNode> &&pattern)
    : PatternOneNode(), identifier_(identifier), pattern_(std::move(pattern)) {}
PatternIDNode::~PatternIDNode() = default;
void PatternIDNode::accept(Visitor &visitor) { visitor.visit(*this); }
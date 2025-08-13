#include "PatternIDNode.hpp"
#include "Visitor.hpp"

PatternIDNode::PatternIDNode(std::unique_ptr<PatternNode> &&pattern,
                             const std::string &identifier)
    : PatternOneNode(), pattern_(std::move(pattern)), identifier_(identifier) {}

void PatternIDNode::accept(Visitor &visitor) { visitor.visit(*this); }
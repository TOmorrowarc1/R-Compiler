#include "PatternIDNode.hpp"
#include "Visitor.hpp"
#include "ValueInfo.hpp"

PatternIDNode::PatternIDNode(const std::string &identifier,
                             std::unique_ptr<PatternNode> &&pattern)
    : PatternNode(), identifier_(identifier), pattern_(std::move(pattern)) {}
PatternIDNode::~PatternIDNode() = default;
void PatternIDNode::accept(Visitor &visitor) { visitor.visit(this); }
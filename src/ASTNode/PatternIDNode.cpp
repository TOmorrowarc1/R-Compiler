#include "PatternIDNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

PatternIDNode::PatternIDNode(const std::string &identifier,
                             std::unique_ptr<PatternNode> &&pattern,
                             Position position)
    : PatternNode(position), identifier_(identifier),
      pattern_(std::move(pattern)) {}
PatternIDNode::~PatternIDNode() = default;
void PatternIDNode::accept(Visitor &visitor) { visitor.visit(this); }
#include "PatternTupleNode.hpp"
#include "Visitor.hpp"
#include "PathNode.hpp"

PatternTupleNode::PatternTupleNode(
    std::unique_ptr<PathNode> path,
    std::vector<std::unique_ptr<PatternNode>> &&pattern)
    : PatternNode(), path_(std::move(path)), patterns_(std::move(pattern)) {}
PatternTupleNode::~PatternTupleNode() = default;
void PatternTupleNode::accept(Visitor &visitor) { visitor.visit(this); }
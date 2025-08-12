#include "TypeTupleNode.hpp"
#include "Visitor.hpp"

TypeTupleNode::TypeTupleNode(std::vector<std::unique_ptr<TypeNode>> &&fields)
    : TypeNode(), fields_(std::move(fields)) {}

void TypeTupleNode::accept(Visitor &visitor) { visitor.visit(*this); }
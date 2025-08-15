#include "TypeSliceNode.hpp"
#include "Visitor.hpp"

TypeSliceNode::TypeSliceNode(std::unique_ptr<TypeNode> &&type)
    : TypeNode(), type_(std::move(type)) {}
TypeSliceNode::~TypeSliceNode() = default;
void TypeSliceNode::accept(Visitor &visitor) { visitor.visit(*this); }
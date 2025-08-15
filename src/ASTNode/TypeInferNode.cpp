#include "TypeInferNode.hpp"
#include "Visitor.hpp"

TypeInferNode::TypeInferNode() : TypeNode() {}
TypeInferNode::~TypeInferNode() = default;
void TypeInferNode::accept(Visitor &visitor) { visitor.visit(*this); }
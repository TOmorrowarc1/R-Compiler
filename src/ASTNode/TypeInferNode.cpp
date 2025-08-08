#include "TypeInferNode.hpp"
#include "Visitor.hpp"

TypeInferNode::TypeInferNode() : TypeNode() {}
void TypeInferNode::accept(Visitor &visitor) { visitor.visit(*this); }
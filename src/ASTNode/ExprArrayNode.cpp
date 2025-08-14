#include "ExprArrayNode.hpp"
#include "Visitor.hpp"

ExprArrayNode::ExprArrayNode(std::vector<std::unique_ptr<ExprNode>> &&elements,
                             std::unique_ptr<ExprNode> &&length)
    : ExprBlockOutNode(), elements_(std::move(elements)),
      length_(std::move(length)) {}

void ExprArrayNode::accept(Visitor &visitor) { visitor.visit(*this); }
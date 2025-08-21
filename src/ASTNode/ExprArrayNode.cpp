#include "ExprArrayNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

ExprArrayNode::ExprArrayNode(std::vector<std::unique_ptr<ExprNode>> &&elements,
                             std::unique_ptr<ExprNode> &&length,
                             Position position)
    : ExprBlockOutNode(position), elements_(std::move(elements)),
      length_(std::move(length)) {}
ExprArrayNode::~ExprArrayNode() = default;
void ExprArrayNode::accept(Visitor &visitor) { visitor.visit(this); }
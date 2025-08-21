#include "ExprCallNode.hpp"
#include "Visitor.hpp"
#include "ValueInfo.hpp"

ExprCallNode::ExprCallNode(std::unique_ptr<ExprNode> &&caller,
                           std::vector<std::unique_ptr<ExprNode>> &&arguments)
    : ExprBlockOutNode(), caller_(std::move(caller)),
      arguments_(std::move(arguments)) {}
ExprCallNode::~ExprCallNode() = default;
void ExprCallNode::accept(Visitor &visitor) { visitor.visit(this); }
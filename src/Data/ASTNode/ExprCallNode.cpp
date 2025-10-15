#include "ExprCallNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

ExprCallNode::ExprCallNode(std::unique_ptr<ExprNode> &&caller,
                           std::vector<std::unique_ptr<ExprNode>> &&arguments,
                           Position position)
    : ExprBlockOutNode(position), caller_(std::move(caller)),
      arguments_(std::move(arguments)) {}
ExprCallNode::~ExprCallNode() = default;
void ExprCallNode::accept(Visitor &visitor) { visitor.visit(this); }
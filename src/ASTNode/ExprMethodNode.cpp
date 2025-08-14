#include "ExprMethodNode.hpp"
#include "Visitor.hpp"

ExprMethodNode::ExprMethodNode(
    std::unique_ptr<ExprNode> &&instance, const std::string &ID,
    std::vector<std::unique_ptr<ExprNode>> &&parameters)
    : ExprBlockOutNode(), instance_(std::move(instance)), ID_(ID),
      parameters_(std::move(parameters)) {}

void ExprMethodNode::accept(Visitor &visitor) { visitor.visit(*this); }
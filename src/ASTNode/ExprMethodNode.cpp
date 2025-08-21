#include "ExprMethodNode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

ExprMethodNode::ExprMethodNode(
    std::unique_ptr<ExprNode> &&instance, const std::string &ID,
    std::vector<std::unique_ptr<ExprNode>> &&parameters, Position position)
    : ExprBlockOutNode(position), instance_(std::move(instance)), ID_(ID),
      parameters_(std::move(parameters)) {}
ExprMethodNode::~ExprMethodNode() = default;
void ExprMethodNode::accept(Visitor &visitor) { visitor.visit(this); }
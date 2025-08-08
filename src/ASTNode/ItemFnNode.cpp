#include "ItemFnNode.hpp"

ItemFnNode::ItemFnNode(const std::string &ID,
                       std::unique_ptr<TypeNode> &&returnType,
                       std::vector<std::unique_ptr<ParameterNode>> &&parameters)
    : ItemNode(), ID_(ID), returnType_(std::move(returnType)),
      parameters_(std::move(parameters)) {}

void ItemFnNode::accept(Visitor &visitor) { visitor.visit(this); }
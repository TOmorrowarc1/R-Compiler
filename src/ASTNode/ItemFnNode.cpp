#include "ItemFnNode.hpp"
#include "Visitor.hpp"

ItemFnNode::ItemFnNode(const std::string &ID,
                       std::unique_ptr<TypeNode> &&returnType,
                       std::vector<FnParameter> &&parameters,
                       std::unique_ptr<ExprBlockNode> &&body)
    : ItemNode(), ID_(ID), returnType_(std::move(returnType)),
      parameters_(std::move(parameters)), body_(std::move(body)) {}

void ItemFnNode::accept(Visitor &visitor) { visitor.visit(*this); }
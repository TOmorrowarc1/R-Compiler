#include "ItemFnNode.hpp"
#include "ExprBlockNode.hpp"
#include "PatternANode.hpp"
#include "TypeANode.hpp"
#include "Visitor.hpp"

ItemFnNode::ItemFnNode(const std::string &ID,
                       std::unique_ptr<TypeNode> &&returnType,
                       std::vector<ItemFnPara> &&parameters,
                       std::unique_ptr<ExprBlockNode> &&body)
    : ItemNode(), ID_(ID), returnType_(std::move(returnType)),
      parameters_(std::move(parameters)), body_(std::move(body)) {}
ItemFnNode::~ItemFnNode() = default;
void ItemFnNode::accept(Visitor &visitor) { visitor.visit(*this); }
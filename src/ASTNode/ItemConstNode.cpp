#include "ItemConstNode.hpp"
#include "ExprANode.hpp"
#include "TypeANode.hpp"
#include "ValueInfo.hpp"
#include "Visitor.hpp"

ItemConstNode::ItemConstNode(const std::string &ID,
                             std::unique_ptr<TypeNode> &&type,
                             std::unique_ptr<ExprNode> &&value,
                             Position position)
    : ItemNode(position), ID_(ID), type_(std::move(type)),
      value_(std::move(value)) {}
ItemConstNode::~ItemConstNode() = default;
void ItemConstNode::accept(Visitor &visitor) { visitor.visit(this); }